#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ltr_gui_prefs.h"
#include "pref.h"
#include "pref_global.h"
#include "utils.h"
#include <map>

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStringList>
#include <vector>
#include <iostream>

PrefProxy *PrefProxy::prf = nullptr;
bool PrefProxy::isAppImage = false;

static int warnMessage(const QString &message) {
  return QMessageBox::warning(nullptr, QString::fromUtf8("Linuxtrack"), message,
                              QMessageBox::Ok, QMessageBox::Ok);
}

static QString normalizeDeviceId(QString id) {
  id = id.trimmed();
  int bracketPos = id.lastIndexOf(QString::fromUtf8(" ["));
  if ((bracketPos > 0) && id.endsWith(QChar::fromLatin1(']'))) {
    id = id.left(bracketPos).trimmed();
  }
  return id;
}

static int deviceIdMatchScore(const QString &lhs, const QString &rhs) {
  const QString lhsNorm = normalizeDeviceId(lhs);
  const QString rhsNorm = normalizeDeviceId(rhs);
  if (lhsNorm.compare(rhsNorm, Qt::CaseInsensitive) == 0) {
    if (rhs.size() > lhs.size()) {
      return 4;
    }
    if (lhs.compare(rhs, Qt::CaseInsensitive) == 0) {
      return 3;
    }
    return 2;
  }

  if (lhs.compare(rhs, Qt::CaseInsensitive) == 0) {
    return 3;
  }

  if (lhsNorm.startsWith(rhsNorm, Qt::CaseInsensitive) ||
      rhsNorm.startsWith(lhsNorm, Qt::CaseInsensitive)) {
    return 1;
  }

  return 0;
}

static bool findDeviceSectionByTypeAndId(const QString &devType,
                                         const QString &devId,
                                         QString &result) {
  std::vector<std::string> sections;
  ltr_int_get_section_list(&sections);
  int bestScore = 0;
  int bestIdLength = -1;
  bool found = false;

  for (const std::string &section : sections) {
    char *devName =
        ltr_int_get_key(section.c_str(), "Capture-device");
    char *devIdStr =
        ltr_int_get_key(section.c_str(), "Capture-device-id");
    if ((devName != nullptr) && (devIdStr != nullptr)) {
      const QString currentType = QString::fromUtf8(devName);
      const QString currentId = QString::fromUtf8(devIdStr);
      const int score =
          (devType.compare(currentType, Qt::CaseInsensitive) == 0)
              ? deviceIdMatchScore(devId, currentId)
              : 0;
      if ((score > bestScore) ||
          ((score == bestScore) && (score > 0) &&
           (currentId.size() > bestIdLength))) {
        result = QString::fromStdString(section);
        bestScore = score;
        bestIdLength = currentId.size();
        found = true;
      }
    }
    free(devName);
    free(devIdStr);
  }

  return found;
}

static QString matchingFaceTrackerType(const QString &deviceType) {
  if (deviceType.compare(QString::fromUtf8("Webcam"), Qt::CaseInsensitive) ==
      0) {
    return QString::fromUtf8("Webcam-face");
  }
  if (deviceType.compare(QString::fromUtf8("MacWebcam"), Qt::CaseInsensitive) ==
      0) {
    return QString::fromUtf8("MacWebcam-face");
  }
  if (deviceType.compare(QString::fromUtf8("Ps3Eye"), Qt::CaseInsensitive) ==
          0 ||
      deviceType.compare(QString::fromUtf8("PS3Eye"), Qt::CaseInsensitive) ==
          0) {
    return QString::fromUtf8("Ps3Eye-face");
  }
  return QString();
}

static QString baseCaptureType(const QString &deviceType) {
  if (deviceType.compare(QString::fromUtf8("Webcam-face"),
                         Qt::CaseInsensitive) == 0) {
    return QString::fromUtf8("Webcam");
  }
  if (deviceType.compare(QString::fromUtf8("MacWebcam-face"),
                         Qt::CaseInsensitive) == 0) {
    return QString::fromUtf8("MacWebcam");
  }
  if (deviceType.compare(QString::fromUtf8("Ps3Eye-face"),
                         Qt::CaseInsensitive) == 0) {
    return QString::fromUtf8("Ps3Eye");
  }
  return QString();
}

static bool recoverFaceTrackerInputIfNeeded() {
  char *modelSection = ltr_int_get_key("Global", "Model");
  if (modelSection == nullptr) {
    return false;
  }
  const QString activeModel = QString::fromUtf8(modelSection);
  free(modelSection);
  if (activeModel.compare(QString::fromUtf8("Face"), Qt::CaseInsensitive) != 0) {
    return false;
  }

  char *inputSection = ltr_int_get_key("Global", "Input");
  if (inputSection == nullptr) {
    return false;
  }
  const QString currentSection = QString::fromUtf8(inputSection);

  char *devName = ltr_int_get_key(inputSection, "Capture-device");
  char *devId = ltr_int_get_key(inputSection, "Capture-device-id");
  free(inputSection);
  if ((devName == nullptr) || (devId == nullptr)) {
    free(devName);
    free(devId);
    return false;
  }

  const QString currentType = QString::fromUtf8(devName);
  const QString currentId = QString::fromUtf8(devId);
  free(devName);
  free(devId);

  QString desiredType = matchingFaceTrackerType(currentType);
  QString lookupId = currentId;
  if (desiredType.isEmpty()) {
    const QString sourceType = baseCaptureType(currentType);
    if (sourceType.isEmpty()) {
      return false;
    }
    desiredType = currentType;

    QString sourceSection;
    if (findDeviceSectionByTypeAndId(sourceType, currentId, sourceSection)) {
      char *sourceId =
          ltr_int_get_key(sourceSection.toUtf8().constData(),
                          "Capture-device-id");
      if (sourceId != nullptr) {
        lookupId = QString::fromUtf8(sourceId);
        free(sourceId);
      }
    }
  }

  if (desiredType.isEmpty()) {
    return false;
  }

  QString recoveredSection;
  if (!findDeviceSectionByTypeAndId(desiredType, lookupId, recoveredSection)) {
    return false;
  }
  if (recoveredSection.compare(currentSection, Qt::CaseInsensitive) == 0) {
    return false;
  }

  ltr_int_log_message(
      "Recovering face-tracker input: '%s' -> '%s' for model '%s'\n",
      currentSection.toUtf8().constData(), recoveredSection.toUtf8().constData(),
      activeModel.toUtf8().constData());
  return ltr_int_change_key("Global", "Input",
                            recoveredSection.toUtf8().constData());
}

PrefProxy::PrefProxy() {
  isAppImage = qEnvironmentVariableIsSet("APPIMAGE") ||
               qEnvironmentVariableIsSet("APPDIR");
  if (ltr_int_read_prefs(nullptr, false)) {
    checkPrefix(true);
    if (recoverFaceTrackerInputIfNeeded()) {
      savePrefs();
    }
    return;
  }
  ltr_int_log_message("Pref file not found, trying linuxtrack.conf\n");
  if (ltr_int_read_prefs("linuxtrack.conf", false)) {
    ltr_int_prefs_changed();
    checkPrefix(true);
    if (recoverFaceTrackerInputIfNeeded()) {
      savePrefs();
    }
    return;
  }

  ltr_int_log_message("Couldn't load preferences (GUI), copying default!\n");
  if (!makeRsrcDir()) {
    throw;
  }
  if (!copyDefaultPrefs()) {
    throw;
  }
  ltr_int_new_prefs();
  ltr_int_read_prefs(nullptr, true);
  checkPrefix(true);
  if (recoverFaceTrackerInputIfNeeded()) {
    savePrefs();
  }
}

bool PrefProxy::checkPrefix(bool save) {
  QString appPath = QApplication::applicationDirPath();

  appPath.prepend(QString::fromUtf8("\""));
  appPath += QString::fromUtf8("\"");

  bool res;
  char *tmp_prefix = ltr_int_get_key("Global", "Prefix");
  if (tmp_prefix != nullptr) {
    res = true;
    prefix = QString::fromStdString(tmp_prefix);
  } else {
    res = false;
    prefix = QString::fromUtf8("");
  }

  // If we have a stable prefix already, and we are running from an AppImage,
  // do NOT overwrite it with the volatile AppImage mount point.
  if (isAppImage && res && !prefix.contains(QString::fromUtf8("/tmp/"))) {
    ltr_int_log_message("Running from AppImage, preserving stable prefix: %s\n",
                        prefix.toUtf8().constData());
    return true;
  }

  if (res && (prefix == appPath)) {
    // Intentionaly left empty
  } else {
    // If we are an AppImage and have no prefix, we should ideally NOT set it to
    // the volatile mount point either, but for now we follow old logic unless
    // we find a better "stable" home.
    prefix = appPath;
    bool res =
        ltr_int_change_key("Global", "Prefix", appPath.toUtf8().constData());
    if (save) {
      res &= savePrefs();
    }
    return res;
  }
  return true;
}

bool PrefProxy::makeRsrcDir() {
  QString msg;
  QString targetDir = PrefProxy::getRsrcDirPath();
  if (targetDir.endsWith(QString::fromUtf8("/"))) {
    targetDir.chop(1);
  }
  QFileInfo rsrcDir(targetDir);
  if (rsrcDir.isDir()) {
    return true;
  }
  if (rsrcDir.exists() || rsrcDir.isSymLink()) {
    QString bck = targetDir + QString::fromUtf8(".pre");
    QFileInfo bckInfo(bck);
    if (bckInfo.exists() || bckInfo.isSymLink()) {
      if (!QFile::remove(bck)) {
        msg = QString(QString::fromUtf8("Can't remove '") + bck +
                      QString::fromUtf8("'!"));
        goto problem;
      }
    }
    if (!QFile::rename(targetDir, bck)) {
      msg =
          QString(QString::fromUtf8("Can't rename '") + targetDir +
                  QString::fromUtf8("' to '") + bck + QString::fromUtf8("'!"));
      goto problem;
    }
  }
  if (!QDir::home().mkpath(targetDir)) {
    msg = QString(QString::fromUtf8("Can't create '") + targetDir +
                  QString::fromUtf8("'!"));
    goto problem;
  }
  return true;
problem:
  ltr_int_log_message(QString(msg + QString::fromUtf8("\n")).toUtf8().data());
  warnMessage(msg);
  return false;
}

bool PrefProxy::copyDefaultPrefs() {
  QString msg;
  // we can assume the rsrc dir exists now...
  QString targetDir = PrefProxy::getRsrcDirPath();
  if (targetDir.endsWith(QString::fromUtf8("/"))) {
    targetDir.chop(1);
  }
  QString target = targetDir + QString::fromUtf8("/linuxtrack1.conf");
  QString source =
      PrefProxy::getDataPath(QString::fromUtf8("linuxtrack1.conf"));
  QFileInfo target_info(target);
  if (target_info.exists() || target_info.isSymLink()) {
    QString bck = target + QString::fromUtf8(".backup");
    QFileInfo bckInfo(bck);
    if (bckInfo.exists() || bckInfo.isSymLink()) {
      if (!QFile::remove(bck)) {
        msg = QString(QString::fromUtf8("Can't remove '") + bck +
                      QString::fromUtf8("'!"));
        goto problem;
      }
    }
    if (!QFile::rename(target, bck)) {
      msg =
          QString(QString::fromUtf8("Can't rename '") + target +
                  QString::fromUtf8("' to '") + bck + QString::fromUtf8("'!"));
      goto problem;
    }
  }
  if (!QFile::copy(source, target)) {
    msg =
        QString(QString::fromUtf8("Can't copy '") + source +
                QString::fromUtf8("' to '") + target + QString::fromUtf8("'!"));
    goto problem;
  }

  return true;
problem:
  ltr_int_log_message(
      QString(msg + QString::fromUtf8("\n")).toUtf8().constData());
  warnMessage(msg);
  return false;
}

PrefProxy::~PrefProxy() {}

PrefProxy &PrefProxy::Pref() {
  if (prf == nullptr) {
    prf = new PrefProxy();
  }
  return *prf;
}

void PrefProxy::ClosePrefs() {
  if (prf != nullptr) {
    delete prf;
    prf = nullptr;
    ltr_int_free_prefs();
  }
}

void PrefProxy::SavePrefsOnExit() {
  if (ltr_int_need_saving()) {
    QMessageBox::StandardButton res;
    res = QMessageBox::warning(
        nullptr, QString::fromUtf8("Linuxtrack"),
        QString::fromUtf8("Preferences were modified,") +
            QString::fromUtf8("Do you want to save them?"),
        QMessageBox::Save | QMessageBox::Close, QMessageBox::Save);
    if (res == QMessageBox::Save) {
      savePrefs();
    }
  }
}

bool PrefProxy::activateDevice(const QString &sectionName) {
  ltr_int_change_key("Global", "Input", sectionName.toUtf8().constData());
  return true;
}

bool PrefProxy::activateModel(const QString &sectionName) {
  ltr_int_change_key("Global", "Model", sectionName.toUtf8().constData());
  return true;
}

bool PrefProxy::createSection(QString &sectionName) {
  char *tmp = ltr_int_add_unique_section(sectionName.toUtf8().constData());
  if (tmp != nullptr) {
    sectionName = QString::fromUtf8(tmp);
    return true;
  } else {
    sectionName = QString::fromUtf8("");
    return false;
  }
}

bool PrefProxy::getKeyVal(const QString &sectionName, const QString &keyName,
                          QString &result) {
  char *val = ltr_int_get_key(sectionName.toUtf8().constData(),
                              keyName.toUtf8().constData());
  if (val != nullptr) {
    result = QString::fromUtf8(val);
    return true;
  } else {
    return false;
  }
}

/*
bool PrefProxy::getKeyVal(const QString &keyName, QString &result)
{
  const char *val = ltr_int_get_key(nullptr, keyName.toUtf8().constData());
  if(val != nullptr){
    result = val;
    return true;
  }else{
    return false;
  }
}
*/

bool PrefProxy::addKeyVal(const QString &sectionName, const QString &keyName,
                          const QString &value) {
  return ltr_int_change_key(sectionName.toUtf8().constData(),
                            keyName.toUtf8().constData(),
                            value.toUtf8().constData());
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
                          const QString &value) {
  return ltr_int_change_key(sectionName.toUtf8().constData(),
                            keyName.toUtf8().constData(),
                            value.toUtf8().constData());
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
                          const int &value) {
  return ltr_int_change_key_int(sectionName.toUtf8().constData(),
                                keyName.toUtf8().constData(), value);
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
                          const float &value) {
  return ltr_int_change_key_flt(sectionName.toUtf8().constData(),
                                keyName.toUtf8().constData(), value);
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
                          const double &value) {
  return ltr_int_change_key_flt(sectionName.toUtf8().constData(),
                                keyName.toUtf8().constData(), value);
}

bool PrefProxy::getFirstDeviceSection(const QString &devType, QString &result) {
  char *devName =
      ltr_int_find_section("Capture-device", devType.toUtf8().constData());
  if (devName != nullptr) {
    result = QString::fromUtf8(devName);
    return true;
  } else {
    return false;
  }
}

bool PrefProxy::getFirstDeviceSection(const QString &devType,
                                      const QString &devId, QString &result) {
  return findDeviceSectionByTypeAndId(devType, devId, result);
}

bool PrefProxy::getActiveDevice(deviceType_t &devType, QString &id,
                                QString &secName) {
  char *devSection = ltr_int_get_key("Global", "Input");
  if (devSection == nullptr) {
    return false;
  }
  char *devName = ltr_int_get_key(devSection, "Capture-device");
  char *devId = ltr_int_get_key(devSection, "Capture-device-id");
  if ((devName == nullptr) || (devId == nullptr)) {
    return false;
  }

  QString dn = QString::fromUtf8(devName);
  if (dn.compare(QString::fromUtf8("Webcam"), Qt::CaseInsensitive) == 0) {
    devType = WEBCAM;
  } else if (dn.compare(QString::fromUtf8("Webcam-face"),
                        Qt::CaseInsensitive) == 0) {
    devType = WEBCAM_FT;
  } else if (dn.compare(QString::fromUtf8("MacWebcam"), Qt::CaseInsensitive) ==
             0) {
    devType = MACWEBCAM;
  } else if (dn.compare(QString::fromUtf8("MacWebcam-face"),
                        Qt::CaseInsensitive) == 0) {
    devType = MACWEBCAM_FT;
  } else if (dn.compare(QString::fromUtf8("Wiimote"), Qt::CaseInsensitive) ==
             0) {
    devType = WIIMOTE;
  } else if (dn.compare(QString::fromUtf8("Tir"), Qt::CaseInsensitive) == 0) {
    devType = TIR;
  } else if (dn.compare(QString::fromUtf8("Tir_openusb"),
                        Qt::CaseInsensitive) == 0) {
    devType = TIR;
  } else if (dn.compare(QString::fromUtf8("PS3Eye"), Qt::CaseInsensitive) ==
             0) {
    devType = MACPS3EYE;
  } else if (dn.compare(QString::fromUtf8("PS3Eye-face"),
                        Qt::CaseInsensitive) == 0) {
    devType = MACPS3EYE_FT;
  } else if (dn.compare(QString::fromUtf8("Joystick"), Qt::CaseInsensitive) ==
             0) {
    devType = JOYSTICK;
  } else {
    devType = NONE;
  }
  id = QString::fromUtf8(devId);
  secName = QString::fromUtf8(devSection);
  return true;
}

bool PrefProxy::getActiveDevice(deviceType_t &devType, QString &id) {
  QString tmp;
  return getActiveDevice(devType, id, tmp);
}

bool PrefProxy::getActiveModel(QString &model) {
  char *modelSection = ltr_int_get_key("Global", "Model");
  if (modelSection == nullptr) {
    return false;
  }
  model = QString::fromUtf8(modelSection);
  return true;
}

bool PrefProxy::getModelList(QStringList &list) {
  std::vector<std::string> sections;
  ltr_int_find_sections("Model-type", (void *)&sections);
  list.clear();
  for (size_t i = 0; i < sections.size(); ++i) {
    list.append(QString::fromStdString(sections[i]));
  }
  return (list.size() != 0);
}

bool PrefProxy::getProfiles(QStringList &list) {
  std::vector<std::string> profiles;
  char *title;
  ltr_int_find_sections("Title", (void *)&profiles);
  list.clear();
  for (size_t i = 0; i < profiles.size(); ++i) {
    title = ltr_int_get_key(profiles[i].c_str(), "Title");
    if (title != nullptr) {
      list.append(QString::fromUtf8(title));
    }
  }
  return (list.size() != 0);
}

bool PrefProxy::getProfileSection(const QString &name, QString &section) {
  char *secName = ltr_int_find_section("Title", name.toUtf8().constData());
  if (secName != nullptr) {
    section = QString::fromUtf8(secName);
    return true;
  }
  return false;
}

bool PrefProxy::savePrefs() {
  bool res = ltr_int_save_prefs(nullptr);
  return res;
}

QString PrefProxy::findRuntimeFile(QString file,
                                   const QStringList &relativeDirs,
                                   bool searchPath) {
  QFileInfo fileInfo(file);
  if (fileInfo.isAbsolute()) {
    return fileInfo.exists() ? fileInfo.absoluteFilePath() : QString();
  }

  QString normalizedFile = file;
  while (normalizedFile.startsWith(QChar::fromLatin1('/'))) {
    normalizedFile.remove(0, 1);
  }

  QStringList roots;
  QString appDirEnv = QString::fromLocal8Bit(qgetenv("APPDIR"));
  if (!appDirEnv.isEmpty()) {
    roots << appDirEnv;
  }
  roots << QApplication::applicationDirPath();

  for (const QString &root : roots) {
    QDir dir(root);
    for (const QString &relativeDir : relativeDirs) {
      QString candidate = relativeDir.isEmpty()
                              ? dir.absoluteFilePath(normalizedFile)
                              : dir.absoluteFilePath(relativeDir +
                                                     QString::fromUtf8("/") +
                                                     normalizedFile);
      if (QFileInfo::exists(candidate)) {
        return candidate;
      }
    }
  }

  if (searchPath) {
    QString pathHit = QStandardPaths::findExecutable(normalizedFile);
    if (!pathHit.isEmpty()) {
      return pathHit;
    }
  }

  return QString();
}

QString PrefProxy::getDataPath(QString file) {
  QString appPath = QApplication::applicationDirPath();
  QString appDirEnv = QString::fromLocal8Bit(qgetenv("APPDIR"));
  QString normalizedFile = file;
  while (normalizedFile.startsWith(QChar::fromLatin1('/'))) {
    normalizedFile.remove(0, 1);
  }

#ifdef DARWIN
  QString bundlePath =
      QDir(appPath).absoluteFilePath(QString::fromUtf8("../Resources/linuxtrack/") +
                                     normalizedFile);
  if (QFileInfo::exists(bundlePath)) {
    return bundlePath;
  }
#endif

  // If running from AppImage, prioritize internal share/linuxtrack folder
  // Check via isAppImage flag OR explicit APPDIR environment variable
  if (isAppImage || !appDirEnv.isEmpty()) {
    QString baseDir = !appDirEnv.isEmpty()
                          ? appDirEnv + QString::fromUtf8("/usr/bin")
                          : appPath;
    QString internalPath = QDir(baseDir).absoluteFilePath(
        QString::fromUtf8("../share/linuxtrack/") + normalizedFile);
    if (QFile::exists(internalPath)) {
      return internalPath;
    }
  }

  char *path = ltr_int_get_data_path_prefix(normalizedFile.toUtf8().constData(),
                                            appPath.toUtf8().constData());
  QString res = QString::fromUtf8(path);
  free(path);
  return res;
  /*
    QString appPath = QApplication::applicationDirPath();
  #ifndef DARWIN
    return appPath + QString::fromUtf8("/../share/linuxtrack/") + file;
  #else
    return appPath + QString::fromUtf8("/../Resources/linuxtrack/") + file;
  #endif
  */
}

QString PrefProxy::getExecutablePath(QString file) {
  return findRuntimeFile(file,
                         QStringList()
                             << QString::fromUtf8("usr/bin")
                             << QString::fromUtf8("")
                             << QString::fromUtf8("..")
                             << QString::fromUtf8("../bin")
                             << QString::fromUtf8("../../src")
                             << QString::fromUtf8("../../bin")
                             << QString::fromUtf8("../Resources/linuxtrack/bin"),
                         true);
}

QString PrefProxy::getHelperPath(QString file) {
  return findRuntimeFile(file,
                         QStringList()
                             << QString::fromUtf8("../Resources/linuxtrack/helper")
                             << QString::fromUtf8("../helper")
                             << QString::fromUtf8("../../helper")
                             << QString::fromUtf8("helper"),
                         true);
}

QString PrefProxy::getLibPath(QString file) {
  char *path = ltr_int_get_lib_path(file.toUtf8().constData());
  QString res = QString::fromUtf8(path);
  free(path);
  return res;
  /*
    QString appPath = QApplication::applicationDirPath();
  #ifndef DARWIN
    return appPath + QString::fromUtf8("/../lib/") + file +
  QString::fromUtf8(".so.0"); #else return appPath +
  QString::fromUtf8("/../Frameworks/") + file + QString::fromUtf8(".0.dylib");
  #endif
  */
}

QString PrefProxy::getPrefix() {
  QString res = prefix;
  if (res.startsWith(QString::fromUtf8("\"")) &&
      res.endsWith(QString::fromUtf8("\""))) {
    res = res.mid(1, res.length() - 2);
  }
  return res;
}

QString PrefProxy::getRsrcDirPath() {
  return QDir::homePath() + QString::fromUtf8("/.config/linuxtrack/");
}

bool PrefProxy::rereadPrefs() {
  ltr_int_free_prefs();
  ltr_int_read_prefs(nullptr, true);
  checkPrefix(true);
  return true;
}

void PrefProxy::announceModelChange() { ltr_int_announce_model_change(); }

void PrefProxy::getSectionList(QStringList &list) {
  std::vector<std::string> tmpList;
  ltr_int_get_section_list((void *)&tmpList);
  list.clear();
  for (size_t i = 0; i < tmpList.size(); ++i) {
    list.append(QString::fromStdString(tmpList[i]));
  }
}

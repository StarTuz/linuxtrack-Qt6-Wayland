#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ltr_gui_prefs.h"
#include "prefix_discovery.h"
#include "utils.h"
#include "xplugin.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

static QMessageBox::StandardButton warningMessage(const QString &message) {
  ltr_int_log_message("XPlane plugin install - %s\n",
                      message.toUtf8().constData());
  return QMessageBox::warning(nullptr, QString::fromUtf8("Linuxtrack"), message,
                              QMessageBox::Ok);
}

static void warn(const QString baseMsg, const QString explanation) {
  ltr_int_log_message("XPlane plugin install - %s(%s)\n",
                      baseMsg.toUtf8().constData(),
                      explanation.toUtf8().constData());
  warningMessage(
      QString::fromUtf8("%1\nSystem says: %2").arg(baseMsg).arg(explanation));
}

static bool removePlugin(const QString targetName) {
  QFile target(targetName);
  if (target.exists()) {
    if (!target.remove()) {
      warn(QString::fromUtf8("Can't remove old plugin '%1'!").arg(targetName),
           target.errorString());
      return false;
    }
  }
  return true;
}

static bool installPlugin(const QString sourceFile, const QString destFile) {
  ltr_int_log_message("Going to install '%s' to '%s'...\n",
                      sourceFile.toUtf8().constData(),
                      destFile.toUtf8().constData());
  // Create destination path
  QFile src(sourceFile);
  QFile dest(destFile);
  if (!src.exists()) {
    warningMessage(
        QString::fromUtf8("Source file '%1' doesn't exist!").arg(sourceFile));
    return false;
  }
  QFileInfo destInfo(destFile);
  QDir destDir = destInfo.dir();
  // make sure the destination path exists
  if (!destDir.exists()) {
    if (!destDir.mkpath(destDir.path())) {
      warningMessage(QString::fromUtf8("Can't create output directory '%1'!")
                         .arg(destDir.path()));
      return false;
    }
  }
  // check if the file exists already
  if (dest.exists()) {
    if (!removePlugin(destFile)) {
      return false;
    }
  }
  // copy the new file
  if (!src.copy(destFile)) {
    warn(QString::fromUtf8("Can't copy file '%1' to '%2'!")
             .arg(destFile)
             .arg(destDir.path()),
         src.errorString());
    return false;
  }
  return true;
}

static bool ensureStableLibraries() {
  if (!PREF.runningFromAppImage()) {
    return true;
  }

  QString prefix = PREF.getPrefix();
  bool isVolatile = prefix.contains(QString::fromUtf8("/tmp/")) ||
                    prefix.contains(QString::fromUtf8(".mount_"));

  if (!isVolatile) {
    return true;
  }

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(
      nullptr, QString::fromUtf8("Linuxtrack AppImage"),
      QString::fromUtf8(
          "You are running from an AppImage. To make the X-Plane "
          "plugin work after closing this AppImage, I need to "
          "install core libraries to a stable location (~/.local).\n\n"
          "Do you want to proceed?"),
      QMessageBox::Yes | QMessageBox::No);
  if (reply != QMessageBox::Yes) {
    return true;
  }

  QString stableLibDir =
      QDir::homePath() + QString::fromUtf8("/.local/lib/linuxtrack");
  QString stableShareDir =
      QDir::homePath() + QString::fromUtf8("/.local/share/linuxtrack");
  QDir dir;
  if (!dir.mkpath(stableLibDir)) {
    warningMessage(
        QString::fromUtf8("Can't create stable library directory '%1'!")
            .arg(stableLibDir));
    return false;
  }
  if (!dir.mkpath(stableShareDir)) {
    warningMessage(
        QString::fromUtf8("Can't create stable share directory '%1'!")
            .arg(stableShareDir));
    return false;
  }

  // Copy all libraries
  QString appLibPath = QApplication::applicationDirPath() +
                       QString::fromUtf8("/../lib/linuxtrack/");
  QDir appLibDir(appLibPath);
  QStringList libs = appLibDir.entryList(QDir::Files);

  foreach (const QString &lib, libs) {
    QString src = appLibPath + lib;
    QString dst = stableLibDir + QString::fromUtf8("/") + lib;
    QFile::remove(dst);
    if (!QFile::copy(src, dst)) {
      ltr_int_log_message("Failed to copy library %s to %s\n",
                          src.toUtf8().constData(), dst.toUtf8().constData());
    }
  }

  // Copy all data files
  QString appSharePath = QApplication::applicationDirPath() +
                         QString::fromUtf8("/../share/linuxtrack/");
  QDir appShareDir(appSharePath);
  QStringList shares = appShareDir.entryList(QDir::Files);

  foreach (const QString &share, shares) {
    QString src = appSharePath + share;
    QString dst = stableShareDir + QString::fromUtf8("/") + share;
    QFile::remove(dst);
    if (!QFile::copy(src, dst)) {
      ltr_int_log_message("Failed to copy data file %s to %s\n",
                          src.toUtf8().constData(), dst.toUtf8().constData());
    }
  }

  // Update prefix to ~/.local/bin (so /../lib/linuxtrack works as expected)
  PREF.setKeyVal(QString::fromUtf8("Global"), QString::fromUtf8("Prefix"),
                 QString::fromUtf8("\"") + QDir::homePath() +
                     QString::fromUtf8("/.local/bin\""));
  PREF.savePrefs();

  QMessageBox::information(
      nullptr, QString::fromUtf8("Linuxtrack"),
      QString::fromUtf8(
          "Stable libraries and data installed to ~/.local.\n"
          "Prefix updated. The X-Plane plugin should now work reliably."));
  return true;
}

void XPluginInstall::on_BrowseXPlane_pressed() {
  if (!ensureStableLibraries()) {
    reject();
    return;
  }

  QString startDir = QDir::homePath();
  QStringList discovered = PrefixDiscovery::discoverXPlane();
  if (!discovered.isEmpty()) {
    // Pick the first one as a sensible default
    startDir = discovered.first();
  } else {
    // Fallback to the specific path for now if discovery fails
    QString userPath = QString::fromUtf8("/xplane/x-plane/X-Plane12/");
    if (QDir(userPath).exists()) {
      startDir = userPath;
    }
  }

  QString fileName = QFileDialog::getOpenFileName(
      this, QString::fromUtf8("Find XPlane executable"), startDir,
      QString::fromUtf8("All Files (*)"));
  if (fileName.isEmpty()) {
    reject();
    return;
  }
  // Use QFileInfo instead of QRegExp for path extraction
  QFileInfo fileInfo(fileName);
  QString destPath = fileInfo.path() + QString::fromUtf8("/Resources/plugins");
  QString sourceFile = PrefProxy::getLibPath(QString::fromUtf8("xlinuxtrack9"));
  if (!QFile::exists(destPath)) {
    warningMessage(
        QString(QString::fromUtf8("Can't install XPlane plugin there:'") +
                fileName + QString::fromUtf8("'")));
    reject();
    return;
  }

  // Check for the old plugin and remove it if exists
  QString oldPlugin = destPath + QString::fromUtf8("/xlinuxtrack.xpl");
  QFileInfo old(oldPlugin);
  if (old.exists()) {
    if (!removePlugin(oldPlugin)) {
      reject();
      return;
    }
  }
  destPath += QString::fromUtf8("/xlinuxtrack/64/");
#ifndef DARWIN
  if (installPlugin(sourceFile, destPath + QString::fromUtf8("/lin.xpl"))) {
#else
  if (installPlugin(sourceFile, destPath + QString::fromUtf8("/mac.xpl"))) {
#endif
    QMessageBox::information(
        nullptr, QString::fromUtf8("Linuxtrack"),
        QString::fromUtf8("XPlane plugin installed successfuly!"));
  } else {
    warningMessage(QString::fromUtf8("XPlane plugin installation failed!"));
    reject();
  }
  accept();
}

#include "moc_xplugin.cpp"

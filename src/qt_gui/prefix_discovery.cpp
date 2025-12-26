#include "prefix_discovery.h"
#include "utils.h"
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTextStream>

QList<DetectedPrefix> PrefixDiscovery::discover() {
  ltr_int_log_message("Starting prefix discovery...\n");
  QList<DetectedPrefix> list;
  scanSteam(list);
  scanLutris(list);
  scanBottles(list);
  scanStardardWine(list);
  ltr_int_log_message("Prefix discovery completed. Total found: %d\n",
                      list.size());
  return list;
}

void PrefixDiscovery::scanSteam(QList<DetectedPrefix> &list) {
  QString home = QDir::homePath();
  QStringList libraryPaths;

  // Add primary fallbacks
  libraryPaths << home + QString::fromUtf8("/.local/share/Steam");

  QStringList potentialVdfs;
  potentialVdfs << home + QString::fromUtf8(
                              "/.steam/steam/steamapps/libraryfolders.vdf")
                << home +
                       QString::fromUtf8(
                           "/.local/share/Steam/steamapps/libraryfolders.vdf");

  for (const QString &vdfPath : potentialVdfs) {
    if (!QFile::exists(vdfPath))
      continue;

    QFile file(vdfPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream in(&file);
      QRegularExpression re(QString::fromUtf8("\"path\"\\s+\"([^\"]+)\""),
                            QRegularExpression::CaseInsensitiveOption);
      while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
          libraryPaths << match.captured(1);
        }
      }
    }
  }

  libraryPaths.removeDuplicates();

  // Resolve the main library path (where compatdata typically lives)
  QString mainCompatData =
      home + QString::fromUtf8("/.local/share/Steam/steamapps/compatdata");

  for (const QString &libPath : libraryPaths) {
    QString steamappsPath = libPath + QString::fromUtf8("/steamapps");
    QDir steamappsDir(steamappsPath);
    if (!steamappsDir.exists()) {
      steamappsPath = libPath + QString::fromUtf8("/SteamApps");
      steamappsDir = QDir(steamappsPath);
      if (!steamappsDir.exists())
        continue;
    }

    ltr_int_log_message("Scanning Steam library: %s\n",
                        steamappsPath.toUtf8().constData());

    // Use QDir::entryInfoList to get all files and filter manually
    QFileInfoList allFiles =
        steamappsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    ltr_int_log_message("  Total files in directory: %d\n", allFiles.size());

    for (const QFileInfo &fileInfo : allFiles) {
      QString fileName = fileInfo.fileName();

      // Check if it's an appmanifest file
      if (!fileName.startsWith(QString::fromUtf8("appmanifest_")) ||
          !fileName.endsWith(QString::fromUtf8(".acf"))) {
        continue;
      }

      QString appId =
          fileName.section(QLatin1Char('_'), 1).section(QLatin1Char('.'), 0, 0);
      if (appId.isEmpty())
        continue;

      // Check multiple possible prefix locations
      QStringList potentialPfxPaths;
      potentialPfxPaths << steamappsPath + QString::fromUtf8("/compatdata/") +
                               appId + QString::fromUtf8("/pfx");
      potentialPfxPaths << mainCompatData + QString::fromUtf8("/") + appId +
                               QString::fromUtf8("/pfx");

      QString activePfx;
      for (const QString &p : potentialPfxPaths) {
        QFileInfo fi(p);
        if (fi.exists() && fi.isDir()) {
          activePfx = p;
          break;
        }
      }

      if (!activePfx.isEmpty()) {
        QString name = getSteamName(fileInfo.absoluteFilePath());
        if (name.isEmpty())
          name = QString::fromUtf8("Steam App ") + appId;

        DetectedPrefix dp;
        dp.name = name;
        dp.path = activePfx;
        dp.type = QString::fromUtf8("Steam/Proton");
        list << dp;
        ltr_int_log_message("  [Steam] Found: %s (%s)\n",
                            name.toUtf8().constData(),
                            activePfx.toUtf8().constData());
      }
    }
  }
}

QString PrefixDiscovery::getSteamName(const QString &manifestPath) {
  QFile file(manifestPath);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    QRegularExpression re(QString::fromUtf8("\"name\"\\s+\"([^\"]+)\""),
                          QRegularExpression::CaseInsensitiveOption);
    while (!in.atEnd()) {
      QString line = in.readLine();
      QRegularExpressionMatch match = re.match(line);
      if (match.hasMatch()) {
        return match.captured(1);
      }
    }
  }
  return QString();
}

void PrefixDiscovery::scanLutris(QList<DetectedPrefix> &list) {
  QString lutrisDir =
      QDir::homePath() + QString::fromUtf8("/.config/lutris/games");
  QDir dir(lutrisDir);
  if (!dir.exists())
    return;

  QStringList files =
      dir.entryList(QStringList() << QString::fromUtf8("*.yml"), QDir::Files);
  for (const QString &file : files) {
    QFile f(dir.absoluteFilePath(file));
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream in(&f);
      QString name, prefix;
      QRegularExpression nameRe(QString::fromUtf8("^name:\\s+(.*)$"));
      QRegularExpression prefixRe(QString::fromUtf8("^\\s+prefix:\\s+(.*)$"));
      while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch m;
        if ((m = nameRe.match(line)).hasMatch())
          name = m.captured(1).trimmed();
        if ((m = prefixRe.match(line)).hasMatch())
          prefix = m.captured(1).trimmed();
      }
      if (!name.isEmpty() && !prefix.isEmpty() && QDir(prefix).exists()) {
        DetectedPrefix dp;
        dp.name = name;
        dp.path = prefix;
        dp.type = QString::fromUtf8("Lutris");
        list << dp;
      }
    }
  }
}

void PrefixDiscovery::scanBottles(QList<DetectedPrefix> &list) {
  QStringList potentialDirs;
  potentialDirs
      << QDir::homePath() +
             QString::fromUtf8(
                 "/.var/app/com.usebottles.bottles/data/bottles/bottles")
      << QDir::homePath() + QString::fromUtf8("/.local/share/bottles/bottles");
  for (const QString &dirPath : potentialDirs) {
    QDir dir(dirPath);
    if (!dir.exists())
      continue;
    QStringList bottles = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &bottle : bottles) {
      QString path = dir.absoluteFilePath(bottle);
      if (QDir(path + QString::fromUtf8("/drive_c")).exists()) {
        DetectedPrefix dp;
        dp.name = bottle;
        dp.path = path;
        dp.type = QString::fromUtf8("Bottles");
        list << dp;
      }
    }
  }
}

void PrefixDiscovery::scanStardardWine(QList<DetectedPrefix> &list) {
  QString dotWine = QDir::homePath() + QString::fromUtf8("/.wine");
  if (QDir(dotWine + QString::fromUtf8("/drive_c")).exists()) {
    DetectedPrefix dp;
    dp.name = QString::fromUtf8("Default Wine Prefix");
    dp.path = dotWine;
    dp.type = QString::fromUtf8("Wine");
    list << dp;
  }
}

QStringList PrefixDiscovery::discoverXPlane() {
  QStringList paths;
  QString home = QDir::homePath();
  QStringList installFiles;
  installFiles << home + QString::fromUtf8("/.x-plane/x-plane_install_12.txt")
               << home + QString::fromUtf8("/.x-plane/x-plane_install_11.txt");
  for (const QString &file : installFiles) {
    if (QFile::exists(file)) {
      QFile f(file);
      if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        while (!in.atEnd()) {
          QString line = in.readLine().trimmed();
          if (line.isEmpty() || line.startsWith(QLatin1Char('#')))
            continue;
          if (QDir(line).exists())
            paths << line;
        }
      }
    }
  }
  return paths;
}

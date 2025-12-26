#include "prefix_discovery.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTextStream>

QList<DetectedPrefix> PrefixDiscovery::discover() {
  QList<DetectedPrefix> list;
  scanSteam(list);
  scanLutris(list);
  scanBottles(list);
  scanStardardWine(list);
  return list;
}

void PrefixDiscovery::scanSteam(QList<DetectedPrefix> &list) {
  QString home = QDir::homePath();
  QStringList potentialVdfs;
  potentialVdfs
      << home + QString::fromUtf8("/.steam/steam/steamapps/libraryfolders.vdf")
      << home + QString::fromUtf8(
                    "/.local/share/Steam/steamapps/libraryfolders.vdf")
      << home + QString::fromUtf8("/.var/app/com.valvesoftware.Steam/.steam/"
                                  "steam/steamapps/libraryfolders.vdf");

  QStringList libraryPaths;
  for (const QString &vdfPath : potentialVdfs) {
    if (!QFile::exists(vdfPath))
      continue;

    QFile file(vdfPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream in(&file);
      QRegularExpression re(QString::fromUtf8("\"path\"\\s+\"([^\"]+)\""));
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

  for (const QString &libPath : libraryPaths) {
    QDir steamapps(libPath + QString::fromUtf8("/steamapps"));
    if (!steamapps.exists())
      continue;

    QStringList manifests = steamapps.entryList(
        QStringList() << QString::fromUtf8("appmanifest_*.acf"), QDir::Files);
    for (const QString &manifest : manifests) {
      QString appId =
          manifest.section(QLatin1Char('_'), 1).section(QLatin1Char('.'), 0);
      QString pfxPath = libPath + QString::fromUtf8("/steamapps/compatdata/") +
                        appId + QString::fromUtf8("/pfx");

      if (QDir(pfxPath).exists()) {
        QString name = getSteamName(steamapps.absoluteFilePath(manifest));
        if (name.isEmpty())
          name = QString::fromUtf8("Steam App ") + appId;

        DetectedPrefix dp;
        dp.name = name;
        dp.path = pfxPath;
        dp.type = QString::fromUtf8("Steam/Proton");
        list << dp;
      }
    }
  }
}

QString PrefixDiscovery::getSteamName(const QString &manifestPath) {
  QFile file(manifestPath);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    QRegularExpression re(QString::fromUtf8("\"name\"\\s+\"([^\"]+)\""));
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

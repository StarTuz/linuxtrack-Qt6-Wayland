#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "ltr_gui.h"
#include "utils.h"
#include <QLocale>
#include <QSurfaceFormat>
#include <locale.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  ltr_int_check_root();
  ltr_int_log_message("Starting ltr_gui\n");
  setenv("LC_ALL", "C.UTF-8", 1);
  setlocale(LC_ALL, "C.UTF-8");
  QLocale::setDefault(QLocale::c());

  // Request Desktop OpenGL and a decent version
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(3, 3);
  format.setAlphaBufferSize(0); // Ensure no transparency
  QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);

  // Command-line argument parsing
  QCommandLineParser parser;
  parser.setApplicationDescription(
      QString::fromLatin1("Linuxtrack GUI - Head tracking for Linux"));
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption autostartOption(
      QStringList() << QString::fromLatin1("a")
                    << QString::fromLatin1("autostart"),
      QString::fromLatin1("Automatically start tracking after initialization"));
  parser.addOption(autostartOption);

  parser.process(app);

  bool autostart = parser.isSet(autostartOption);

  LinuxtrackGui gui(nullptr, autostart);
  gui.show();
  return app.exec();
}

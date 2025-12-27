#include <QApplication>
#include <QSurfaceFormat>

#include "ltr_gui.h"
#include "utils.h"
#include <locale.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  ltr_int_check_root();
  ltr_int_log_message("Starting ltr_gui\n");
  setenv("LC_ALL", "C", 1);
  setlocale(LC_ALL, "C");

  // Force Qt6 to use OpenGL backend for RHI, avoiding software rendering
  // issues that cause "non-opengl surface" errors
  setenv("QT_QUICK_BACKEND", "software", 0); // Don't override if set
  setenv("QSG_RHI_BACKEND", "opengl", 0);    // Prefer OpenGL for RHI

  // Set default surface format for OpenGL compatibility
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 0);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QLocale::setDefault(QLocale::c());
  QApplication app(argc, argv);
  LinuxtrackGui gui;
  gui.show();
  return app.exec();
}

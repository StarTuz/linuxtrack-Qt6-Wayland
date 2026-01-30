#include "utils.h"
#include <QApplication>
#include <QLocale>
#include <locale.h>
#include <stdlib.h>

#include "mickey.h"

int main(int argc, char *argv[]) {
  ltr_int_check_root();
  ltr_int_log_message("Starting Mickey controller!\n");
  setenv("LC_ALL", "C.UTF-8", 1);
  setlocale(LC_ALL, "C.UTF-8");
  QLocale::setDefault(QLocale::c());
  QApplication app(argc, argv);
  GUI.show();
  int res = app.exec();
  MickeyGUI::deleteInstance();
  return res;
}

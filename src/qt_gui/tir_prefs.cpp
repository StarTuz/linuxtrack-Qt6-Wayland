#include "tir_prefs.h"
#include "dyn_load.h"
#include "ltr_gui_prefs.h"
#include "pathconfig.h"
#include "tir_driver_prefs.h"
#include "ui_tir_setup.h"
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <iostream>

static QString currentId = QString::fromUtf8("None");
static int tirType = 0;
bool TirPrefs::firmwareOK = false;
bool TirPrefs::permsOK = false;

typedef int (*probe_tir_fun_t)(bool *have_firmware, bool *have_permissions);
static probe_tir_fun_t probe_tir_fun = nullptr;
static lib_fun_def_t functions[] = {
    {(char *)"ltr_int_tir_found", (void *)&probe_tir_fun}, {nullptr, nullptr}};

static int probeTir(bool &fwOK, bool &permOK) {
  void *libhandle = nullptr;
  int res = 0;
  if ((libhandle = ltr_int_load_library((char *)"libtir", functions)) !=
      nullptr) {
    res = probe_tir_fun(&fwOK, &permOK);
    ltr_int_unload_library(libhandle, functions);
  }
  return res;
}

/*
void TirPrefs::Connect()
{
  QObject::connect(ui.TirThreshold, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirThreshold_valueChanged(int)));
  QObject::connect(ui.TirMinBlob, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirMinBlob_valueChanged(int)));
  QObject::connect(ui.TirMaxBlob, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirMaxBlob_valueChanged(int)));
  QObject::connect(ui.TirStatusBright, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirStatusBright_valueChanged(int)));
  QObject::connect(ui.TirIrBright, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirIrBright_valueChanged(int)));
  QObject::connect(ui.TirSignalizeStatus, SIGNAL(stateChanged(int)),
    this, SLOT(on_TirSignalizeStatus_stateChanged(int)));
  QObject::connect(ui.TirInstallFirmware, SIGNAL(pressed()),
    this, SLOT(on_TirInstallFirmware_pressed()));
}
*/

TirPrefs::TirPrefs(const QString &dev_id, QWidget *parent)
    : QWidget(parent), id(dev_id), dlfw(nullptr) {
  ui.setupUi(this);
  // Connect();
  Activate(id, true);
}

TirPrefs::~TirPrefs() {
  // std::cout<<"Destructing tirprefs!\n";
  if (dlfw != nullptr) {
    // std::cout<<"Closing dlfw!\n";
    dlfw->close();
    delete dlfw;
  }
}

bool TirPrefs::Activate(const QString &ID, bool init) {
  initializing = init;
  QString sec;
  if (PREF.getFirstDeviceSection(QString::fromUtf8("Tir"), sec)) {
    QString currentDev, currentSection;
    deviceType_t devType;
    if (!PREF.getActiveDevice(devType, currentDev, currentSection) ||
        (sec != currentSection)) {
      PREF.activateDevice(sec);
    }
  } else {
    sec = QString::fromUtf8("TrackIR");
    initializing = false;
    if (PREF.createSection(sec)) {
      PREF.addKeyVal(sec, QString::fromUtf8("Capture-device"),
                     QString::fromUtf8("Tir"));
      PREF.addKeyVal(sec, QString::fromUtf8("Capture-device-id"), ID);
      PREF.addKeyVal(sec, QString::fromUtf8("Threshold"), QString::number(140));
      PREF.addKeyVal(sec, QString::fromUtf8("Min-blob"), QString::number(4));
      PREF.addKeyVal(sec, QString::fromUtf8("Max-blob"), QString::number(2500));
      PREF.addKeyVal(sec, QString::fromUtf8("Status-led-brightness"),
                     QString::number(0));
      PREF.addKeyVal(sec, QString::fromUtf8("Ir-led-brightness"),
                     QString::number(7));
      PREF.addKeyVal(sec, QString::fromUtf8("Status-signals"),
                     QString::fromUtf8("on"));
      PREF.addKeyVal(sec, QString::fromUtf8("Grayscale"),
                     QString::fromUtf8("on"));
      PREF.activateDevice(sec);
    } else {
      return false;
    }
  }
  ltr_int_tir_init_prefs();
  currentId = ID;
  ui.TirThreshold->setValue(ltr_int_tir_get_threshold());
  ui.TirMaxBlob->setValue(ltr_int_tir_get_max_blob());
  ui.TirMinBlob->setValue(ltr_int_tir_get_min_blob());
  ui.TirIrBright->setValue(ltr_int_tir_get_ir_brightness());
  ui.TirStatusBright->setValue(ltr_int_tir_get_status_brightness());
  Qt::CheckState state =
      (ltr_int_tir_get_status_indication()) ? Qt::Checked : Qt::Unchecked;
  ui.TirSignalizeStatus->setCheckState(state);
  Qt::CheckState grayscale =
      (ltr_int_tir_get_use_grayscale()) ? Qt::Checked : Qt::Unchecked;
  ui.TirUseGrayscale->setCheckState(grayscale);
  ui.TirVideoOnDelay->setValue(ltr_int_tir_get_video_on_delay());
  if (firmwareOK) {
    if ((tirType < TIR4) || (tirType == TIR5V3)) {
      ui.TirFwLabel->setText(QString::fromUtf8("Firmware not needed!"));
    } else {
      ui.TirFwLabel->setText(QString::fromUtf8("Firmware found!"));
      ui.TirInstallFirmware->setText(QString::fromUtf8("Reinstall Firmware"));
    }
    // ui.TirInstallFirmware->setDisabled(true);
  } else {
    ui.TirFwLabel->setText(
        QString::fromUtf8("Firmware not found - TrackIr will not work!"));
    QMessageBox::warning(
        nullptr, QString::fromUtf8("TrackIR Firmware Installation"),
        QString::fromUtf8("TrackIR device was found, but you don't have the "
                          "firmware installed."));
    // on_TirInstallFirmware_pressed();
  }
  printf("Type: %d\n", tirType);
  if (ui.TirIrBright == nullptr) {
    std::cerr << "TirPrefs: ui.TirIrBright is NULL!\n";
    return false;
  }
  if (ui.TirUseGrayscale == nullptr) {
    std::cerr << "TirPrefs: ui.TirUseGrayscale is NULL!\n";
    return false;
  }

  if ((tirType < TIR5) || (tirType == SMARTNAV4)) {
    ui.TirIrBright->setDisabled(true);
    ui.TirIrBright->setHidden(true);
    ui.TirStatusBright->setDisabled(true);
    ui.TirStatusBright->setHidden(true);
    ui.StatusBrightLabel->setHidden(true);
    ui.StatusBrightLabelOff->setHidden(true);
    ui.StatusBrightLabelBright->setHidden(true);
    ui.IRBrightLabel->setHidden(true);
    ui.IRBrightLabelLow->setHidden(true);
    ui.IRBrightLabelHigh->setHidden(true);
  }
  if (tirType != SMARTNAV4) {
    ui.TirUseGrayscale->setDisabled(true);
    ui.TirUseGrayscale->setHidden(true);
    ui.TirUseGrayscaleLabel->setHidden(true);
  }
  if (tirType == SMARTNAV3) {
    ui.TirThreshold->setMinimum(40);
    ui.TirThresholdMin->setText(QString::fromUtf8("40"));
  } else {
    ui.TirThreshold->setMinimum(30);
    ui.TirThresholdMin->setText(QString::fromUtf8("30"));
  }
  initializing = false;
  return true;
}

bool TirPrefs::AddAvailableDevices(QComboBox &combo) {
  bool res = false;
  QString id;
  deviceType_t dt;
  bool tir_selected = false;

  tirType = probeTir(firmwareOK, permsOK);
  if (!permsOK) {
    QMessageBox::warning(
        nullptr, QString::fromUtf8("TrackIR permissions problem"),
        QString::fromUtf8(
            "TrackIR device was found, but you don't have permissions to access it.\n\n"
            "Troubleshooting steps:\n"
            "1. Install udev rules: sudo cp 99-TIR.rules /etc/udev/rules.d/\n"
            "2. Reload rules: sudo udevadm control --reload-rules && sudo udevadm trigger\n"
            "3. Unplug and replug the device\n"
            "4. Check if USBGuard is blocking access: systemctl status usbguard\n\n"
            "Use the 'Check USB Permissions' button in the Troubleshooting section\n"
            "for detailed diagnostics."));
    return false;
  }
  if (tirType == 0) {
    return res;
  }

  if (PREF.getActiveDevice(dt, id)) {
    if (dt == TIR) {
      tir_selected = true;
    }
  }

  PrefsLink *pl = new PrefsLink(TIR, QString::fromUtf8("Tir"));
  QVariant v;
  v.setValue(*pl);
  combo.addItem(QString::fromUtf8("TrackIR/SmartNav"), v);
  if (tir_selected) {
    combo.setCurrentIndex(combo.count() - 1);
    res = true;
  }
  return res;
}

void TirPrefs::on_TirThreshold_valueChanged(int i) {
  if (!initializing)
    ltr_int_tir_set_threshold(i);
}

void TirPrefs::on_TirMinBlob_valueChanged(int i) {
  if (!initializing)
    ltr_int_tir_set_min_blob(i);
}

void TirPrefs::on_TirMaxBlob_valueChanged(int i) {
  if (!initializing)
    ltr_int_tir_set_max_blob(i);
}

void TirPrefs::on_TirStatusBright_valueChanged(int i) {
  if (!initializing)
    ltr_int_tir_set_status_brightness(i);
}

void TirPrefs::on_TirIrBright_valueChanged(int i) {
  if (!initializing)
    ltr_int_tir_set_ir_brightness(i);
}

void TirPrefs::on_TirSignalizeStatus_stateChanged(int state) {
  if (!initializing)
    ltr_int_tir_set_status_indication(state == Qt::Checked);
}

void TirPrefs::on_TirUseGrayscale_stateChanged(int state) {
  if (!initializing)
    ltr_int_tir_set_use_grayscale(state == Qt::Checked);
}

void TirPrefs::on_TirVideoOnDelay_valueChanged(int i) {
  if (!initializing)
    ltr_int_tir_set_video_on_delay(i);
}

void TirPrefs::on_TirCheckUsb_pressed()
{
  QString report = QString::fromUtf8("Searching for NaturalPoint devices (Vendor ID 131d)...\n\n");
  QProcess lsusb;
  lsusb.start(QString::fromUtf8("lsusb"), QStringList() << QString::fromUtf8("-d") << QString::fromUtf8("131d:"));
  lsusb.waitForFinished();
  QString output = QString::fromUtf8(lsusb.readAllStandardOutput());

  if (output.isEmpty()) {
    report += QString::fromUtf8("Result: No NaturalPoint devices found.\n"
                                "Please ensure your TrackIR/SmartNav is plugged in.");
  } else {
    report += QString::fromUtf8("Result: Found device(s):\n") + output + QString::fromUtf8("\n");
    report += QString::fromUtf8("Checking permissions on /dev/bus/usb/...\n");

    // Parse lsusb output to find bus and device numbers
    // Example: Bus 001 Device 008: ID 131d:0158 NaturalPoint ...
    QTextStream scanner(&output);
    QString line;
    bool anyPermIssue = false;
    while (scanner.readLineInto(&line)) {
      QStringList parts = line.split(QString::fromUtf8(" "), Qt::SkipEmptyParts);
      if (parts.size() >= 4 && parts[0] == QString::fromUtf8("Bus") && parts[2] == QString::fromUtf8("Device")) {
        QString bus = parts[1];
        QString dev = parts[3].left(3); // Remove trailing colon
        QString path = QString::fromUtf8("/dev/bus/usb/%1/%2").arg(bus).arg(dev);
        
        QFileInfo info(path);
        if (info.exists()) {
          report += QString::fromUtf8("\nDevice: %1\n").arg(path);
          report += QString::fromUtf8("  Owner: %1, Group: %2\n").arg(info.owner()).arg(info.group());
          report += QString::fromUtf8("  Permissions: %1\n").arg(QString::number(info.permissions().toInt(), 16));
          
          if (!info.isReadable() || !info.isWritable()) {
            report += QString::fromUtf8("  !!! PERMISSION DENIED: You cannot read/write this device. !!!\n");
            anyPermIssue = true;
          } else {
            report += QString::fromUtf8("  Permissions OK: Access granted.\n");
          }
        }
      }
    }
    
    if (anyPermIssue) {
      report += QString::fromUtf8("\nRecommendation:\n"
                                  "1. Check if 99-TIR.rules is in /etc/udev/rules.d/\n"
                                  "2. Check if your user is in the 'plugdev' or 'usb' group (if applicable)\n"
                                  "3. Check for USBGuard interference: systemctl status usbguard");
    }
  }

  QMessageBox msgBox;
  msgBox.setWindowTitle(QString::fromUtf8("USB Diagnostic Report"));
  msgBox.setText(report);
  msgBox.exec();
}

void TirPrefs::TirFirmwareDLFinished(bool state) {
  if (state) {
    dlfw->hide();
    probeTir(firmwareOK, permsOK);
    if (firmwareOK) {
      ui.TirFwLabel->setText(QString::fromUtf8("Firmware found!"));
      // ui.TirInstallFirmware->setDisabled(true);
      ui.TirInstallFirmware->setText(QString::fromUtf8("Reinstall Firmware"));
    } else {
      ui.TirFwLabel->setText(
          QString::fromUtf8("Firmware not found - TrackIr will not work!"));
    }
  }
}

void TirPrefs::on_TirInstallFirmware_pressed() {
  if (dlfw == nullptr) {
    dlfw = new TirFwExtractor(this);
    QObject::connect(dlfw, SIGNAL(finished(bool)), this,
                     SLOT(TirFirmwareDLFinished(bool)));
  }
  dlfw->show();
  dlfw->raise();
}

#include "moc_tir_prefs.cpp"

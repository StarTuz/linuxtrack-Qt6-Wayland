#include <QMessageBox>
#include <iostream>
#include <QByteArray>
#include "ui_l_wcft_setup.h"
#include "webcam_ft_prefs.h"
#include "wc_driver_prefs.h"
#include "webcam_info.h"
#include "ltr_gui_prefs.h"

static QString currentId = QString::fromUtf8("None");

WebcamFtPrefs::WebcamFtPrefs(const QString &dev_id, QWidget *parent) : QWidget(parent), id(dev_id)
{
  ui.setupUi(this);
  Activate(id, true);
}

WebcamFtPrefs::~WebcamFtPrefs()
{
  ltr_int_wc_close_prefs();
}


static WebcamInfo *wc_info = nullptr;

void WebcamFtPrefs::on_WebcamFtFormats_activated(int index)
{
  ui.WebcamFtResolutions->clear();
  if(currentId == QString::fromUtf8("None")){
    //std::cout<<"None!\n";
    return;
  }
  ui.WebcamFtResolutions->addItems(wc_info->getResolutions(index));
  int res_index = 0;
  int res_x, res_y;
  int fps_num, fps_den;
  if(ltr_int_wc_get_resolution(&res_x, &res_y) &&
    ltr_int_wc_get_fps(&fps_num, &fps_den)){
    res_index = wc_info->findRes(res_x, res_y, fps_num, fps_den, 
				 wc_info->getFourcc(index));
    ui.WebcamFtResolutions->setCurrentIndex(res_index);
  }
  on_WebcamFtResolutions_activated(res_index);
}

void WebcamFtPrefs::on_WebcamFtResolutions_activated(int index)
{
  if(ui.WebcamFtFormats->currentIndex() == -1){
    return;
  }
  QString res, fps, fmt;
  if(wc_info->findFmtSpecs(ui.WebcamFtFormats->currentIndex(), 
                           index, res, fps, fmt)){
    int x,y, num, den;
    WebcamInfo::decodeRes(res, x, y);
    WebcamInfo::decodeFps(fps, num, den);
    if(!initializing){
      ltr_int_wc_set_pixfmt(fmt.toUtf8().constData());
      ltr_int_wc_set_resolution(x, y);
      ltr_int_wc_set_fps(num, den);
    }
  }
}

bool WebcamFtPrefs::Activate(const QString &ID, bool init)
{
  bool res = false;
  QString sec;
  initializing = init;
  if(PREF.getFirstDeviceSection(QString::fromUtf8("Webcam-face"), ID, sec)){
    // Always activate the device when selected
    PREF.activateDevice(sec);
  }else{
    sec = QString::fromUtf8("Webcam-face");
    initializing = false;
    if(PREF.createSection(sec)){
      PREF.addKeyVal(sec, QString::fromUtf8("Capture-device"), QString::fromUtf8("Webcam-face"));
      PREF.addKeyVal(sec, QString::fromUtf8("Capture-device-id"), ID);
      PREF.addKeyVal(sec, QString::fromUtf8("Pixel-format"), QString::fromUtf8(""));
      PREF.addKeyVal(sec, QString::fromUtf8("Resolution"), QString::fromUtf8(""));
      PREF.addKeyVal(sec, QString::fromUtf8("Fps"), QString::fromUtf8(""));

      PREF.activateDevice(sec);
    }else{
      return false;
    }
  }
  if(!ltr_int_wc_init_prefs()){
    initializing = false;
    return false;
  }
  currentId = ID;
  ui.WebcamFtFormats->clear();
  ui.WebcamFtResolutions->clear();
  if((currentId != QString::fromUtf8("None")) && (currentId.size() != 0)){
    if(wc_info != nullptr){
      delete(wc_info);
    }
    wc_info = new WebcamInfo(currentId);
    
    
    ui.WebcamFtFormats->addItems(wc_info->getFormats());
    QString fourcc, thres, bmin, bmax, res, fps, flip;
    int fmt_index = 0;
    const char *tmp = ltr_int_wc_get_pixfmt();
    if(tmp != nullptr){
      fourcc = QString::fromUtf8(tmp);
      fmt_index = wc_info->findFourcc(fourcc);
      ui.WebcamFtFormats->setCurrentIndex(fmt_index);
    }
    on_WebcamFtFormats_activated(fmt_index);

    int beta_val = (int)(ltr_int_wc_get_eff() * 1000.0f);
    ui.ExpFilterFactor->setValue(beta_val);
    on_ExpFilterFactor_valueChanged(beta_val);
    int thresh_val = (int)(ltr_int_wc_get_confidence_threshold() * 100.0f);
    ui.ConfidenceThreshold->setValue(thresh_val);
  }
  initializing = false;
  return res;
}

bool WebcamFtPrefs::AddAvailableDevices(QComboBox &combo)
{
  bool res = false;
  QString id;
  deviceType_t dt;
  bool webcam_selected = false;
  if(PREF.getActiveDevice(dt,id) && (dt == WEBCAM_FT)){
    webcam_selected = true;
  }
  
  QStringList &webcams = WebcamInfo::EnumerateWebcams();
  QStringList::iterator i;
  PrefsLink *pl;
  QVariant v;
  for(i = webcams.begin(); i != webcams.end(); ++i){
    pl = new PrefsLink(WEBCAM_FT, *i);
    v.setValue(*pl);
    combo.addItem((*i)+QString::fromUtf8(" face tracker"), v);
    if(webcam_selected && (*i == id)){
      combo.setCurrentIndex(combo.count() - 1);
      res = true;
    }
  }
  delete(&webcams);
  return res;
}





void WebcamFtPrefs::on_ExpFilterFactor_valueChanged(int value)
{
  float beta = value / 1000.0f; // Map 0-100 to 0.0 - 0.1
  if(!initializing){
    ltr_int_wc_set_eff(beta);
  }
}

void WebcamFtPrefs::on_ConfidenceThreshold_valueChanged(int value)
{
  if(!initializing){
    ltr_int_wc_set_confidence_threshold(value / 100.0f);
  }
}


#include "moc_webcam_ft_prefs.cpp"


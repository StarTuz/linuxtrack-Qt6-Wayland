#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QThread>
#include <QPainter>
#include <QSettings>
#include <iostream>
#include <ltr_show.h>
#include <ltr_gui_prefs.h>
#include "cal.h"
#include "utils.h"
#include <pref_global.h>
#include "pref.hpp"
#include "tracking.h"
#include <iostream>
#include <scp_form.h>
#include <ltr_state.h>
#include <string.h>

#include <linuxtrack.h>
#include <ltr_server.h>
#include "ipc_utils.h"
#include <unistd.h>
#include <tracker.h>

#include "buffering.h"

QWidget *label;
static bool running = false;
static bool camViewEnable = true;
static bool showProcessedView = false;
static int cnt = 0;
static int frames = 0;
static float fps_buffer[8] ={0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static int fps_ptr = 0;
//!!!TBD multithread sync!!!

static QString deviceTypeText(deviceType_t devType)
{
  switch(devType){
    case WEBCAM:
      return QString::fromUtf8("Webcam");
    case WEBCAM_FT:
      return QString::fromUtf8("Webcam Face Tracker");
    case TIR:
      return QString::fromUtf8("TrackIR");
    case WIIMOTE:
      return QString::fromUtf8("Wiimote");
    case JOYSTICK:
      return QString::fromUtf8("Joystick");
    case MACWEBCAM:
      return QString::fromUtf8("Mac Webcam");
    case MACWEBCAM_FT:
      return QString::fromUtf8("Mac Webcam Face Tracker");
    case MACPS3EYE:
      return QString::fromUtf8("PS3Eye");
    case MACPS3EYE_FT:
      return QString::fromUtf8("PS3Eye Face Tracker");
    default:
      return QString::fromUtf8("Unknown device");
  }
}

static QString cameraViewModeText(deviceType_t devType, const QString &modelType)
{
  switch(devType){
    case WEBCAM:
    case MACWEBCAM:
    case MACPS3EYE:
      if(modelType.compare(QString::fromUtf8("Face"), Qt::CaseInsensitive) == 0){
        return QString::fromUtf8("Camera View can show either the grayscale preview or the processed tracking mask. This device/model combination is not a native face-tracker path.");
      }
      return QString::fromUtf8("Camera View can show either the grayscale preview or the processed tracking mask for blob tracking.");
    case WEBCAM_FT:
    case MACWEBCAM_FT:
    case MACPS3EYE_FT:
      return QString::fromUtf8("Camera View can show either the grayscale preview or the processed image used by the face-tracker pipeline.");
    case TIR:
      return QString::fromUtf8("Camera View shows the processed sensor frame used for point tracking.");
    case WIIMOTE:
      return QString::fromUtf8("Camera View is diagnostic tracking output from the selected device path.");
    case JOYSTICK:
      return QString::fromUtf8("Joystick tracking does not provide a meaningful camera preview.");
    default:
      return QString::fromUtf8("Camera View is a diagnostic tracking view.");
  }
}



LtrGuiForm::LtrGuiForm(const Ui::LinuxtrackMainForm &tmp_gui, QSettings &settings)
              : glw(nullptr), cv(nullptr), allowClose(false), main_gui(tmp_gui)
{
  ui.setupUi(this);
  cv = new CameraView(label);
  ui.pix_box->addWidget(cv);
  trackerStopped();
  settings.beginGroup(QString::fromUtf8("TrackingWindow"));
  camViewEnable = ! settings.value(QString::fromUtf8("camera_view"), false).toBool();
  bool check3DV = settings.value(QString::fromUtf8("3D_view"), false).toBool();
  showProcessedView =
      settings.value(QString::fromUtf8("preview_mode"), 0).toInt() == 1;
  settings.endGroup();
  main_gui.DisableCamView->setCheckState(camViewEnable ? Qt::Unchecked : Qt::Checked);
  main_gui.Disable3DView->setCheckState(check3DV ? Qt::Checked : Qt::Unchecked);
  ui.previewMode->setCurrentIndex(showProcessedView ? 1 : 0);
  connect(ui.previewMode, SIGNAL(currentIndexChanged(int)),
          this, SLOT(previewModeChanged(int)));
  cv->setProcessedView(showProcessedView);
  updateCameraDiagnostics();
  glw = new Window(ui.tabWidget, main_gui.Disable3DView);
  ui.ogl_box->addWidget(glw);
  timer = new QTimer(this);
  fpsTimer = new QTimer(this);
  stopwatch = new QElapsedTimer();
  frames = 0;
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  connect(fpsTimer, SIGNAL(timeout()), this, SLOT(updateFps()));
  if(!connect(&TRACKER, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)))){
    //std::cout<<"Problem connecting signal1!\n";
  }
  if(!connect(&TRACKER, SIGNAL(newFrame(struct frame_type *)), 
              this, SLOT(newFrameDelivered(struct frame_type *)))){
    //std::cout<<"Problem connecting signal2!\n";
  }
  connect(main_gui.DisableCamView, SIGNAL(stateChanged(int)), 
          this, SLOT(disableCamView_stateChanged(int)));
  connect(main_gui.Disable3DView, SIGNAL(stateChanged(int)), 
          this, SLOT(disable3DView_stateChanged(int)));
}

//Assuming that frame dimensions can't change while running!!!
void LtrGuiForm::newFrameDelivered(struct frame_type *frame)
{
  if(cnt == 0){
    TRACKER.recenter();
  }
  ++cnt;
  ++frames;
  updateCameraDiagnostics(frame);
}


void LtrGuiForm::updateFps()
{
  int msec = stopwatch->restart();
  if(msec > 0){
    fps_buffer[fps_ptr] = 1000 * frames / msec;
    fps_ptr = (fps_ptr + 1) & 7;
    frames = 0;
  }
}

LtrGuiForm::~LtrGuiForm()
{
  if(running){
    TRACKER.stop();
  }
  delete glw;
}

void LtrGuiForm::StorePrefs(QSettings &settings)
{
  bool camEna = (main_gui.DisableCamView->checkState() == Qt::Checked) ? true : false;
  bool tdEna = (main_gui.Disable3DView->checkState() == Qt::Checked) ? true : false;
  settings.beginGroup(QString::fromUtf8("TrackingWindow"));
  settings.setValue(QString::fromUtf8("camera_view"), camEna);
  settings.setValue(QString::fromUtf8("3D_view"), tdEna);
  settings.setValue(QString::fromUtf8("preview_mode"), showProcessedView ? 1 : 0);
  settings.endGroup();
}



void LtrGuiForm::on_startButton_pressed()
{
  timer->start(50);
  fpsTimer->start(250);
  stopwatch->start();
  static QString sec(QString::fromUtf8("Default"));
  TRACKER.start(sec);
}

void LtrGuiForm::startTracking()
{
  on_startButton_pressed();
}

void LtrGuiForm::on_recenterButton_pressed()
{
  TRACKER.recenter();
}

void LtrGuiForm::on_pauseButton_pressed()
{
  TRACKER.pause();
}

void LtrGuiForm::on_wakeButton_pressed()
{
  TRACKER.wakeup();
}


void LtrGuiForm::on_stopButton_pressed()
{
  TRACKER.stop();
  timer->stop();
  fpsTimer->stop();
}

void LtrGuiForm::disableCamView_stateChanged(int state)
{
  if(state == Qt::Checked){
    camViewEnable = false;
  }else{
    camViewEnable = true;
  }
  updateCameraDiagnostics();
}

void LtrGuiForm::disable3DView_stateChanged(int state)
{
  if(state == Qt::Checked){
    glw->close_widget();
  }else{
    glw->prepare_widget();
  }
}

void LtrGuiForm::previewModeChanged(int index)
{
  showProcessedView = (index == 1);
  cv->setProcessedView(showProcessedView);
  updateCameraDiagnostics();
}

void LtrGuiForm::update()
{
  float fps_mean = 0.0f;
  int i;
  for(i = 0; i < 8; ++i){
    fps_mean += fps_buffer[i];
  }
  int fps = fps_mean / 8.0;
  ui.status->setText(QString::fromUtf8("%1.frame @ %2 fps").arg(cnt).arg(fps, 4));
  cv->redraw();  
}

void LtrGuiForm::updateCameraDiagnostics(const struct frame_type *frame)
{
  if(frame != nullptr){
    latestCameraDiag = QString::fromUtf8(frame->camera_diag);
    latestCameraDiag2 = QString::fromUtf8(frame->camera_diag2);
  }
  updateCameraDiagnostics();
}

void LtrGuiForm::updateCameraDiagnostics()
{
  deviceType_t devType = NONE;
  QString deviceId;
  QString deviceSection;
  QString modelSection;
  QString modelType = QString::fromUtf8("Unknown");
  QString modelLabel = QString::fromUtf8("Unknown");
  QStringList parts;

  if(PREF.getActiveDevice(devType, deviceId, deviceSection)){
    parts << QString::fromUtf8("Device: %1").arg(deviceTypeText(devType));
    if(!deviceId.isEmpty()){
      parts << QString::fromUtf8("ID: %1").arg(deviceId);
    }
  }else{
    parts << QString::fromUtf8("Device: Unknown");
  }

  if(PREF.getActiveModel(modelSection)){
    modelLabel = modelSection;
    QString tmp;
    if(PREF.getKeyVal(modelSection, QString::fromUtf8("Model-type"), tmp)){
      modelType = tmp;
    }
  }
  parts << QString::fromUtf8("Model: %1").arg(modelLabel);

  QString diag = parts.join(QString::fromUtf8(" | "));
  diag += QString::fromUtf8("\n");
  diag += cameraViewModeText(devType, modelType);
  diag += QString::fromUtf8(" Preview mode: %1.").arg(previewModeText());
  if(!latestCameraDiag.isEmpty()){
    diag += QString::fromUtf8("\n");
    diag += latestCameraDiag;
    if(!latestCameraDiag2.isEmpty()){
      diag += QString::fromUtf8(" | ");
      diag += latestCameraDiag2;
    }
  }
  if(!camViewEnable){
    diag += QString::fromUtf8(" Camera View is currently disabled.");
  }
  ui.cameraStatus->setText(diag);
}

QString LtrGuiForm::previewModeText() const
{
  return showProcessedView ? QString::fromUtf8("Processed mask")
                           : QString::fromUtf8("Preview grayscale");
}

void LtrGuiForm::stateChanged(int current_state)
{
  updateCameraDiagnostics();
  switch(current_state){
    case INITIALIZING:
    case RUNNING:
      trackerRunning();
      break;
    case PAUSED:
      trackerPaused();
      break;
    default:
      trackerStopped();
      break;
  }
}

void LtrGuiForm::trackerStopped()
{
  running = false;
  ui.startButton->setDisabled(false);
  ui.pauseButton->setDisabled(true);
  ui.wakeButton->setDisabled(true);
  ui.stopButton->setDisabled(true);
  ui.recenterButton->setDisabled(true);
}

void LtrGuiForm::trackerRunning()
{
  running = true;
  ui.startButton->setDisabled(true);
  ui.pauseButton->setDisabled(false);
  ui.wakeButton->setDisabled(true);
  ui.stopButton->setDisabled(false);
  ui.recenterButton->setDisabled(false);
}

void LtrGuiForm::trackerPaused()
{
  running = true;
  ui.startButton->setDisabled(true);
  ui.pauseButton->setDisabled(true);
  ui.wakeButton->setDisabled(false);
  ui.stopButton->setDisabled(false);
  ui.recenterButton->setDisabled(true);
}

void LtrGuiForm::closeEvent(QCloseEvent *event)
{
  if(allowClose){
    event->accept();
  }else{
    event->ignore();
  }
}

void LtrGuiForm::allowCloseWindow()
{
  allowClose = true;
}

void LtrGuiForm::on_tabWidget_currentChanged(int index)
{
  if(glw == nullptr){
    return;
  }
  if(index == 1){
    glw->resumeUpdates();
  }else{
    glw->stopUpdates();
  }
}


CameraView::CameraView(QWidget *parent)
  : QWidget(parent), processedView(false)
{
  scene = new QGraphicsScene();
  item = new QGraphicsPixmapItem();
  scene->addItem(item);
  view = new QGraphicsView();
  view->setScene(scene);
  layout = new QVBoxLayout();
  layout->addWidget(view);
  setLayout(layout);
}

CameraView::~CameraView()
{
}

void CameraView::setProcessedView(bool processed)
{
  processedView = processed;
}

void CameraView::redraw()
{
  if(!camViewEnable){
    return;
  }
  buffer *b;
  buffering *buf = TRACKER.getBuffers(processedView);
  buffering *fallback = TRACKER.getBuffers(false);
  if(buf->readBuffer(&b)){
    item->setPixmap(QPixmap::fromImage(*(b->getImage())));
    buf->bufferRead();
  }else if(processedView && fallback->readBuffer(&b)){
    item->setPixmap(QPixmap::fromImage(*(b->getImage())));
    fallback->bufferRead();
  }
}

#include "moc_ltr_show.cpp"

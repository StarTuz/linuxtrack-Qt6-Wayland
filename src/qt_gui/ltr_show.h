#ifndef LTR_SHOW__H
#define LTR_SHOW__H

#include "ui_ltr_gui.h"
#include "ui_ltr.h"
#include <linuxtrack.h>
#include "cal.h"
#include <QThread>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QElapsedTimer>

#include "window.h"
#include "camera_view_widget.h"  // New camera preview widget

class LtrGuiForm;
class QSettings;

// CameraView is now replaced by CameraViewWidget in camera_view_widget.h

class LtrGuiForm : public QWidget
{
   Q_OBJECT
  public:
   LtrGuiForm(const Ui::LinuxtrackMainForm &tmp_gui, QSettings &settings);
   ~LtrGuiForm();
   void allowCloseWindow();
   void StorePrefs(QSettings &settings);
   void startTracking();  // Public method for autostart
  public slots:
   void update();
   void updateFps();
    
  private slots:
   void on_startButton_pressed();
   void on_recenterButton_pressed();
   void on_pauseButton_pressed();
   void on_wakeButton_pressed();
   void on_stopButton_pressed();
   void disableCamView_stateChanged(int state);
   void disable3DView_stateChanged(int state);
   void stateChanged(int current_state);
   void newFrameDelivered(struct frame_type *frame);
   void on_tabWidget_currentChanged(int index);
  protected:
   void closeEvent(QCloseEvent *event);
  private:
   Ui::Ltr_gui ui;
   Window *glw;
   QTimer *timer;
   QTimer *fpsTimer;
   QElapsedTimer *stopwatch;
   CameraViewWidget *cv;  // Camera preview widget
   bool allowClose;
   float fps;
   const Ui::LinuxtrackMainForm &main_gui;
   void trackerStopped();
   void trackerRunning();
   void trackerPaused();
};

#endif

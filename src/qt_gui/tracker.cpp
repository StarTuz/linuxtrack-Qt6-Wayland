#include "tracker.h"
#include "axis.h"
#include "ltr_gui_prefs.h"
#include "pref.hpp"
#include "tracking.h"
#include <../ltr_srv_master.h>
#include <../ltr_srv_slave.h>
#include <QApplication>
#include <QMessageBox>
#include <cstdio>
#include <iostream>
#include <ltlib.h>
#include <unistd.h>
#include <utils.h>

Tracker *Tracker::trr = nullptr;
char *com_fname = nullptr;

class MasterThread : public QThread {
public:
  void run();
};

void MasterThread::run() { ltr_int_master(false); }

Tracker &Tracker::trackerInst() {
  if (trr == nullptr) {
    trr = new Tracker();
  }
  return *trr;
}

static void ltr_int_new_slave(char *name) { TRACKER.signalNewSlave(name); }

static void ltr_int_state_changed(void *param) {
  (void)param;
  TRACKER.signalStateChange(ltr_int_get_tracking_state());
}

static buffering buf(3);
static bool initBuffers = true;
static void ltr_int_new_frame(struct frame_type *frame, void *param) {
  (void)param;
  static struct frame_type local_frame;
  local_frame.bloblist = frame->bloblist;
  local_frame.width = frame->width;
  local_frame.height = frame->height;
  local_frame.counter = frame->counter;

  // Save the incoming bitmap for CameraViewWidget BEFORE buffer manipulation
  local_frame.bitmap = frame->bitmap;

  if (initBuffers) {
    buf.resizeBuffers(frame->width, frame->height);
    initBuffers = false;
  }

  if (frame->bitmap != nullptr) {
    buf.bufferWritten();
  }
  buffer *b;
  if (buf.writeBuffer(&b)) {
    frame->bitmap = b->getBuffer();
  } else {
    frame->bitmap = nullptr;
  }

  TRACKER.signalNewFrame(&local_frame);
  static linuxtrack_full_pose_t current_pose;
  ltr_int_get_camera_update(&current_pose);
  TRACKER.signalNewPose(&current_pose);
}

buffering *Tracker::getBuffers() { return &buf; }

Tracker::Tracker()
    : axes(LTR_AXES_T_INITIALIZER), axes_valid(false),
      currentProfile(QString::fromUtf8("Default")), common_ff(0.0) {
  if (!ltr_int_gui_lock(true)) {
    QMessageBox::warning(
        nullptr, QString::fromUtf8("Linuxtrack"),
        QString::fromUtf8("Another linuxtrack gui is running already!"),
        QMessageBox::Ok);
  }
  master = new MasterThread();
  ltr_int_set_callback_hooks(ltr_int_new_frame, ltr_int_state_changed,
                             ltr_int_new_slave);
  setProfile(QString::fromUtf8("Default"));
  axes_valid = true;
}

Tracker::~Tracker() {
  if (master->isRunning()) {
    ltr_int_request_shutdown();
    master->wait();
  }
}

void Tracker::signalStateChange(linuxtrack_state_type current_state) {
  emit stateChanged(current_state);
}

void Tracker::signalNewFrame(struct frame_type *frame) { emit newFrame(frame); }

void Tracker::signalNewPose(linuxtrack_full_pose_t *full_pose) {
  static linuxtrack_pose_t processed;
  static linuxtrack_pose_t unfiltered;
  processed = full_pose->pose;
  ltr_int_postprocess_axes(axes, &processed, &unfiltered);
  // std::cout<<"TRACKER: "<<pose->pitch<<" "<<unfiltered.pitch<<"
  // "<<processed.pitch<<"\n";
  emit newPose(full_pose, &unfiltered, &processed);
}

void Tracker::signalNewSlave(const char *name) {
  QString slaveName = QString::fromUtf8(name);
  QString targetProfile = currentProfile;

  ltr_int_log_message(
      "GUI: New slave connected: '%s' (current GUI profile: '%s')\n", name,
      targetProfile.toUtf8().constData());

  // If slave registered with a different name than current profile, switch it!
  if (slaveName != targetProfile) {
    ltr_int_log_message("GUI: Re-routing slave '%s' -> '%s'\n",
                        slaveName.toUtf8().constData(),
                        targetProfile.toUtf8().constData());
    ltr_int_change_profile(slaveName.toUtf8().constData(),
                           targetProfile.toUtf8().constData());
  }

  // Sync the *current* active settings to the slave
  // We use this->axes because it contains live changes not yet saved to disk
  QByteArray profileNameArray = targetProfile.toUtf8();
  const char *pName = profileNameArray.constData();

  for (int i = PITCH; i <= TZ; ++i) {
    ltr_int_change(pName, i, AXIS_ENABLED,
                   ltr_int_get_axis_bool_param(axes, (axis_t)i, AXIS_ENABLED)
                       ? 1.0f
                       : 0.0f);
    ltr_int_change(pName, i, AXIS_INVERTED,
                   ltr_int_get_axis_bool_param(axes, (axis_t)i, AXIS_INVERTED)
                       ? 1.0f
                       : 0.0f);
    for (int j = AXIS_DEADZONE; j <= AXIS_FILTER; ++j) {
      ltr_int_change(pName, i, j,
                     ltr_int_get_axis_param(axes, (axis_t)i, (axis_param_t)j));
    }
  }
  ltr_int_change(nullptr, MISC, MISC_LEGR, ltr_int_use_oldrot() ? 1.0f : 0.0f);
  ltr_int_change(nullptr, MISC, MISC_ALTER, ltr_int_use_alter() ? 1.0f : 0.0f);
  ltr_int_change(nullptr, MISC, MISC_ALIGN,
                 ltr_int_do_tr_align() ? 1.0f : 0.0f);
  ltr_int_change(nullptr, MISC, MISC_FOCAL_LENGTH, ltr_int_get_focal_length());

  ltr_int_log_message("GUI: Sync to slave '%s' complete.\n", pName);
}

void Tracker::setProfile(QString p) {
  axes_valid = false;

  // Notify connected slaves about profile change (for live switching)
  if (!currentProfile.isEmpty() && currentProfile != p) {
    ltr_int_change_profile(currentProfile.toUtf8().constData(),
                           p.toUtf8().constData());
  }

  ltr_int_close_axes(&axes);
  // PREF.setCustomSection(p);
  currentProfile = p;
  PREF.getProfileSection(currentProfile, profileSection);
  // std::cout<<"Set profile "<<currentProfile.toStdString()<<" -
  // "<<p.toStdString()<<"\n";
  ltr_int_init_axes(&axes, currentProfile.toUtf8().constData());

  common_ff = 1.0;
  int i;
  for (i = PITCH; i <= TZ; ++i) {
    ffs[i] = ltr_int_get_axis_param(axes, (axis_t)i, AXIS_FILTER);
    if (common_ff > ffs[i]) {
      common_ff = ffs[i];
    }
  }
  for (i = PITCH; i <= TZ; ++i) {
    ffs[i] -= common_ff;
  }
  axes_valid = true;
  emit initAxes();
}

void Tracker::fromDefault() {
  ltr_int_axes_from_default(&axes);
  emit initAxes();
}

void Tracker::start(QString &section) {
  (void)section;
  initBuffers = true;
  buf.init();
  master->start();
}

void Tracker::pause() { ltr_int_suspend_cmd(); }

void Tracker::wakeup() { ltr_int_wakeup_cmd(); }

void Tracker::recenter() { ltr_int_recenter_cmd(); }

void Tracker::stop() {
  ltr_int_request_shutdown();
  if (master->isRunning()) {
    ltr_int_request_shutdown();
    master->wait();
  }
}

bool Tracker::axisChange(axis_t axis, axis_param_t elem, bool enabled) {
  ltr_int_set_axis_bool_param(axes, axis, elem, enabled);
  emit axisChanged(axis, elem);
  // Slaves register with profile name (currentProfile), so we must send changes
  // using profile name
  ltr_int_change(currentProfile.toUtf8().constData(), axis, elem,
                 enabled ? 1.0 : 0.0);
  return true;
}

bool Tracker::miscChange(axis_param_t elem, bool enabled) {
  ltr_int_change(nullptr, MISC, elem, enabled ? 1.0 : 0.0);
  return true;
}

bool Tracker::miscChange(axis_param_t elem, float val) {
  ltr_int_change(nullptr, MISC, elem, val);
  return true;
}

static float limit_ff(float val) {
  if (val < 0.0)
    return 0.0;
  if (val > LTR_AXIS_FILTER_MAX)
    return LTR_AXIS_FILTER_MAX;
  return val;
}

bool Tracker::axisChange(axis_t axis, axis_param_t elem, float val) {
  if (elem == AXIS_FILTER) {
    ffs[axis] = val;
    val = limit_ff(val + common_ff);
  }
  bool res = ltr_int_set_axis_param(axes, axis, elem, val);
  emit axisChanged(axis, elem);
  // Slaves register with profile name (currentProfile), so we must send changes
  // using profile name
  ltr_int_change(currentProfile.toUtf8().constData(), axis, elem, val);
  return res;
}

bool Tracker::axisGetBool(axis_t axis, axis_param_t elem) {
  return ltr_int_get_axis_bool_param(axes, axis, elem);
}

float Tracker::axisGet(axis_t axis, axis_param_t elem) {
  if (elem == AXIS_FILTER) {
    return ffs[axis];
  } else {
    return ltr_int_get_axis_param(axes, axis, elem);
  }
}

float Tracker::axisGetValue(axis_t axis, float val) {
  return ltr_int_val_on_axis(axes, axis, val);
}

bool Tracker::axisIsSymetrical(axis_t axis) {
  return ltr_int_is_symetrical(axes, axis);
}

bool Tracker::setCommonFilterFactor(float c_f) {
  common_ff = c_f;
  bool res = true;
  float val;
  for (int i = PITCH; i <= TZ; ++i) {
    val = limit_ff(ffs[i] + common_ff);
    res &= ltr_int_set_axis_param(axes, (axis_t)i, AXIS_FILTER, val);
    emit axisChanged(i, AXIS_FILTER);
    // Slaves register with profile name (currentProfile), so we must send
    // changes using profile name
    ltr_int_change(currentProfile.toUtf8().constData(), i, AXIS_FILTER, val);
  }
  emit setCommonFF(c_f);
  return res;
}

float Tracker::getCommonFilterFactor() { return common_ff; }

#include "moc_tracker.cpp"

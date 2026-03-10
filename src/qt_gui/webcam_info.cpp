#include "webcam_info.h"
#include "dyn_load.h"
#include "list.h"
#include "webcam_driver.h"

#include <QtGlobal>
#include <assert.h>
#include <iostream>
#include <stdint.h>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif
#include <QtDebug>

static bool isNativeFormat(const QString &fourcc) {
  return (fourcc == QString::fromUtf8("YUYV")) ||
         (fourcc == QString::fromUtf8("YU12")) ||
         (fourcc == QString::fromUtf8("YV12")) ||
         (fourcc == QString::fromUtf8("RGB3")) ||
         (fourcc == QString::fromUtf8("BGR3"));
}

static bool isCompressedFormat(const QString &fourcc) {
  return (fourcc == QString::fromUtf8("MJPG")) ||
         (fourcc == QString::fromUtf8("JPEG"));
}

typedef int (*enum_webcams_fun_t)(char **ids[]);
typedef int (*enum_webcam_formats_fun_t)(const char *id,
                                         webcam_formats *all_formats);
typedef int (*enum_webcam_formats_cleanup_fun_t)(webcam_formats *all_formats);

static enum_webcams_fun_t enum_webcams_fun = nullptr;
static enum_webcam_formats_fun_t enum_webcam_formats_fun = nullptr;
static enum_webcam_formats_cleanup_fun_t enum_webcam_formats_cleanup_fun = nullptr;
static lib_fun_def_t functions[] = {
    {(char *)"ltr_int_enum_webcams", (void *)&enum_webcams_fun},
    {(char *)"ltr_int_enum_webcam_formats", (void *)&enum_webcam_formats_fun},
    {(char *)"ltr_int_enum_webcam_formats_cleanup",
     (void *)&enum_webcam_formats_cleanup_fun},
    {nullptr, nullptr}};

/*
 * Bastardized version of singleton pattern to take care of library
 * loading/unloading...
 */
class WebcamLibProxy {
private:
  void *libhandle;
  WebcamLibProxy();
  ~WebcamLibProxy();
  WebcamLibProxy(const WebcamLibProxy &) {};
  WebcamLibProxy &operator=(const WebcamLibProxy &);
  static WebcamLibProxy wcl;
};

WebcamLibProxy WebcamLibProxy::wcl;
bool webcamInfoOk = false;

WebcamLibProxy::WebcamLibProxy() {
  if ((libhandle = ltr_int_load_library((char *)"libwc", functions)) != nullptr) {
    webcamInfoOk = true;
  }
}

WebcamLibProxy::~WebcamLibProxy() {
  if (webcamInfoOk) {
    ltr_int_unload_library(libhandle, functions);
  }
  libhandle = nullptr;
}

// YUYV fourcc: four bytes 'Y','U','Y','V' packed little-endian into uint32_t
static const uint32_t kYUYV = ('Y') | ('U' << 8) | ('Y' << 16) | ('V' << 24);
static webcam_format def_fmt1 = {0, kYUYV, 160, 120, 1, 30};
static webcam_format def_fmt2 = {0, kYUYV, 320, 240, 1, 30};
static webcam_format def_fmt3 = {0, kYUYV, 352, 288, 1, 30};

WebcamInfo::WebcamInfo(const QString &id) {
  if (!webcamInfoOk) {
    throw(0);
  }
  webcam_id = id;
  enum_webcam_formats_fun(webcam_id.toUtf8().constData(), &fmts);

  int j;
  fmt_index = -1;
  int index;
  QString item, pixfmt, width, height, fps;

  if (fmts.entries == 0) {
    // std::cout<<"Zero entries!\n";
    format_strings.push_back(QString::fromUtf8("YUYV"));
    fmt_descs.push_back(QList<webcam_format *>());
    res_list.push_back(QStringList());
    fmt_descs[0].push_back(&def_fmt1);
    fmt_descs[0].push_back(&def_fmt2);
    fmt_descs[0].push_back(&def_fmt3);
    res_list[0].push_back(QString::fromUtf8("160 x 120 @ 30"));
    res_list[0].push_back(QString::fromUtf8("320 x 240 @ 30"));
    res_list[0].push_back(QString::fromUtf8("352 x 288 @ 30"));
  } else {
    for (j = 0; j < fmts.entries; ++j) {
      index = fmts.formats[j].i;
      if (fmt_index != index) {
        fmt_index = index;
        format_strings.push_back(getFormatDisplayName(index));
        fmt_descs.push_back(QList<webcam_format *>());
        res_list.push_back(QStringList());
      }
      fmt_descs.back().push_back(&(fmts.formats[j]));
      width = QString::number(fmts.formats[j].w);
      height = QString::number(fmts.formats[j].h);
      fps = QString::number((float)fmts.formats[j].fps_den /
                            fmts.formats[j].fps_num);
      item = QString::fromUtf8("%1 x %2 @ %3").arg(width, height, fps);
      res_list.back().push_back(item);
    }
  }
}

const QStringList &WebcamInfo::getFormats() { return format_strings; }

QString WebcamInfo::getFormatDisplayName(int index) const {
  const QString fourcc = getFourcc(index);
  if (isNativeFormat(fourcc)) {
    return QString::fromUtf8("%1 (Native)").arg(fourcc);
  }
  if (isCompressedFormat(fourcc)) {
    return QString::fromUtf8("%1 (Converted)").arg(fourcc);
  }
  return QString::fromUtf8("%1 (Driver-dependent)").arg(fourcc);
}

const QStringList &WebcamInfo::getResolutions(int index) {
  // std::cout<<"Selecting format index "<<index<<"\n";
  if ((index >= 0) && (index <= fmt_index)) {
    return res_list[index];
  } else {
    return res_list[0];
  }
}

static QString U32_2_String(uint32_t fourcc) {
  char *fcc = (char *)&(fourcc);
  char fcc1[5];
  qstrncpy(fcc1, fcc, 5);
  fcc1[4] = '\0';
  return QString(QString::fromUtf8(fcc1));
}

QString WebcamInfo::getFourcc(int index) const {
  if ((index >= 0) && (index <= fmt_index)) {
    return U32_2_String(fmt_descs[index][0]->fourcc);
  } else {
    return U32_2_String(*"YUYV");
  }
}

int WebcamInfo::findFourcc(const QString &fcc) {
  for (int i = 0; i < fmt_descs.length(); ++i) {
    if (getFourcc(i) == fcc) {
      return i;
    }
  }
  return 0;
}

typedef struct {
  int w, h;
  int fps_num, fps_den;
} res_struct;

bool WebcamInfo::decodeRes(const QString &res, int &res_x, int &res_y) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  static QRegularExpression res_rexp(
      QString::fromUtf8("^\\s*(\\d+)\\s*[xX]\\s*(\\d+)\\s*$"));
  QRegularExpressionMatch match = res_rexp.match(res);
  if (!match.hasMatch()) {
    return false;
  }
  res_x = match.captured(1).toInt();
  res_y = match.captured(2).toInt();
#else
  const QRegExp &res_rexp =
      QRegExp(QString::fromUtf8("^\\s*(\\d+)\\s*[xX]\\s*(\\d+)\\s*$"));
  if (res_rexp.indexIn(res) == -1) {
    return false;
  }
  res_x = res_rexp.cap(1).toInt();
  res_y = res_rexp.cap(2).toInt();
#endif
  return true;
}

bool WebcamInfo::decodeFps(const QString &fps, int &num, int &den) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  static QRegularExpression fps_rexp(
      QString::fromUtf8("^(\\d+)\\s*/\\s*(\\d+)\\s*$"));
  QRegularExpressionMatch match = fps_rexp.match(fps);
  if (!match.hasMatch()) {
    return false;
  }
  num = match.captured(1).toInt();
  den = match.captured(2).toInt();
#else
  const QRegExp &fps_rexp =
      QRegExp(QString::fromUtf8("^(\\d+)\\s*/\\s*(\\d+)\\s*$"));
  if (fps_rexp.indexIn(fps) == -1) {
    return false;
  }
  num = fps_rexp.cap(1).toInt();
  den = fps_rexp.cap(2).toInt();
#endif
  return true;
}

/*
static bool decodeRes(const QString &res, const QString &fps, res_struct
&decoded)
{
  const QRegExp &res_rexp = QRegExp("^\\s*(\\d+)\\s*[xX]\\s*(\\d+)\\s*$");
  const QRegExp &fps_rexp = QRegExp("^(\\d+)\\s*-- remove! --/\\s*(\\d+)\\s*$");

  if(res_rexp.indexIn(res) == -1){
    return false;
  }
  if(fps_rexp.indexIn(fps) == -1){
    return false;
  }
  decoded.w = res_rexp.cap(1).toInt();
  decoded.h = res_rexp.cap(2).toInt();
  decoded.fps_num = fps_rexp.cap(1).toInt();
  decoded.fps_den = fps_rexp.cap(2).toInt();
  return true;
}
*/

int WebcamInfo::findRes(const int &res_x, const int &res_y, const int &fps_num,
                        const int &fps_den, const QString &fourcc) {
  res_struct fmt = {res_x, res_y, fps_num, fps_den};
  int index = findFourcc(fourcc);
  QList<webcam_format *>::const_iterator i;
  int counter = 0;
  webcam_format *tested;
  for (i = (fmt_descs[index]).begin(); i != (fmt_descs[index]).end();
       ++i, ++counter) {
    tested = *i;
    if ((tested->w == fmt.w) && (tested->h == fmt.h) &&
        (tested->fps_num == fmt.fps_num) && (tested->fps_den == fmt.fps_den)) {
      if (U32_2_String(tested->fourcc) == fourcc) {
        return counter;
      }
    }
  }
  return 0;
}

bool WebcamInfo::findFmtSpecs(int i_fmt, int i_res, QString &res, QString &fps,
                              QString &fmt) {
  if ((i_fmt >= fmt_descs.size()) || (i_res >= fmt_descs[i_fmt].size())) {
    return false;
  }
  webcam_format *format = fmt_descs[i_fmt][i_res];
  res = QString::fromUtf8("%1 x %2")
            .arg(QString::number(format->w))
            .arg(QString::number(format->h));
  fps = QString::fromUtf8("%1/%2")
            .arg(QString::number(format->fps_den))
            .arg(QString::number(format->fps_num));
  fmt = getFourcc(i_fmt);
  return true;
}

QString WebcamInfo::describeFormatPolicy(const QString &fourcc,
                                         bool faceTracker) {
  if (isNativeFormat(fourcc)) {
    if (faceTracker) {
      return QString::fromUtf8(
          "Native backend path. Best-supported for preview and face tracking.");
    }
    return QString::fromUtf8(
        "Native backend path. Best-supported for preview and blob tracking.");
  }
  if (isCompressedFormat(fourcc)) {
    return QString::fromUtf8(
        "Compressed camera mode. Often required for high-FPS webcams, but "
        "Linuxtrack depends on driver/libv4l conversion and the active mode "
        "may differ from the requested one.");
  }
  return QString::fromUtf8(
      "Driver-dependent format. Linuxtrack may rely on driver-side "
      "conversion; if preview is black or tracking is unstable, prefer YUYV.");
}

QString WebcamInfo::describeResolutionPolicy(int i_fmt, int i_res,
                                             bool faceTracker) const {
  if ((i_fmt < 0) || (i_fmt >= fmt_descs.size()) || (i_res < 0) ||
      (i_res >= fmt_descs[i_fmt].size())) {
    return QString();
  }

  webcam_format *format = fmt_descs[i_fmt][i_res];
  const QString fourcc = getFourcc(i_fmt);
  float fps = 0.0f;
  if (format->fps_num > 0) {
    fps = (float)format->fps_den / (float)format->fps_num;
  }

  if (fps >= 90.0f && isCompressedFormat(fourcc)) {
    return QString::fromUtf8(
        "High requested FPS on a compressed stream. Check Camera View "
        "diagnostics after Start for the active mode and driver-reported FPS.");
  }
  if (fps >= 90.0f) {
    return QString::fromUtf8(
        "High requested FPS. Camera diagnostics will show whether the driver "
        "actually negotiated this rate.");
  }
  if (faceTracker && fps < 30.0f) {
    return QString::fromUtf8(
        "Low requested FPS for face tracking. Tracking quality may feel laggy.");
  }
  return QString();
}

WebcamInfo::~WebcamInfo() { enum_webcam_formats_cleanup_fun(&fmts); }

QStringList WebcamInfo::EnumerateWebcams() {
  QStringList res;
  if (!webcamInfoOk) {
    return res;
  }
  char **ids = nullptr;

  if (enum_webcams_fun(&ids) > 0) {
    int id_num = 0;

    while ((ids[id_num]) != nullptr) {
      res.append(QString::fromUtf8(ids[id_num]));
      ++id_num;
    }
    ltr_int_array_cleanup(&ids);
  }
  return res;
}

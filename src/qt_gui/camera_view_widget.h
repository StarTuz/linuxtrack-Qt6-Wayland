#ifndef CAMERA_VIEW_WIDGET_H
#define CAMERA_VIEW_WIDGET_H

#include <QWidget>
#include <QImage>
#include <QMutex>
#include "cal.h"

class CameraViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraViewWidget(QWidget *parent = nullptr);
    ~CameraViewWidget();

public slots:
    void updateFrame(struct frame_type *frame);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage frameImage;
    QMutex imageMutex;
    int lastWidth = 0;
    int lastHeight = 0;
};

#endif // CAMERA_VIEW_WIDGET_H

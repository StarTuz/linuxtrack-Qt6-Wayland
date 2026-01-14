#include "camera_view_widget.h"
#include "tracker.h"
#include <QPainter>
#include <QDebug>

CameraViewWidget::CameraViewWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(320, 240);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAutoFillBackground(true);
    
    // Set black background
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    
    // Connect to tracker's newFrame signal
    connect(&TRACKER, SIGNAL(newFrame(struct frame_type *)),
            this, SLOT(updateFrame(struct frame_type *)));
}

CameraViewWidget::~CameraViewWidget()
{
}

void CameraViewWidget::updateFrame(struct frame_type *frame)
{
    if (!frame || !frame->bitmap) {
        return;
    }
    
    QMutexLocker locker(&imageMutex);
    
    int w = frame->width;
    int h = frame->height;
    
    // Handle dimension changes
    if (w != lastWidth || h != lastHeight || frameImage.isNull()) {
        lastWidth = w;
        lastHeight = h;
        // Create QImage for grayscale (Format_Grayscale8)
        frameImage = QImage(w, h, QImage::Format_Grayscale8);
    }
    
    // Copy bitmap data to QImage
    // frame->bitmap is grayscale uint8_t array
    for (int y = 0; y < h; ++y) {
        memcpy(frameImage.scanLine(y), frame->bitmap + y * w, w);
    }
    
    // Request repaint
    update();
}

void CameraViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    QMutexLocker locker(&imageMutex);
    
    if (frameImage.isNull()) {
        // No frame yet - draw black background with text
        painter.fillRect(rect(), Qt::black);
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, QString::fromUtf8("Camera preview"));
        return;
    }
    
    // Scale image to fit widget while maintaining aspect ratio
    QImage scaled = frameImage.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    // Center the image
    int x = (width() - scaled.width()) / 2;
    int y = (height() - scaled.height()) / 2;
    
    // Draw black background
    painter.fillRect(rect(), Qt::black);
    
    // Draw the frame
    painter.drawImage(x, y, scaled);
}

#include "moc_camera_view_widget.cpp"

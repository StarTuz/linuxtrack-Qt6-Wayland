#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGlobal>
#include <memory>

// Qt6: Use modern OpenGL with shaders (required for Wayland/EGL)
#include <QMatrix4x4>
#include <QMutex>
#include <QMutexLocker>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#define GLWIDGET_BASE QOpenGLWidget

#include <QThread>

class ReaderThread : public QThread {
  Q_OBJECT
public:
  ReaderThread();
  void run();
signals:
  void done();

private:
};

class GLWidget : public GLWIDGET_BASE, protected QOpenGLFunctions {
  Q_OBJECT

public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
signals:
  void ready();
public slots:
  void setXRotation(float angle);
  void setYRotation(float angle);
  void setZRotation(float angle);
  void setXTrans(float pos);
  void setYTrans(float pos);
  void setZTrans(float pos);
  void objectsRead();

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

private:
  bool makeObjects();
  void normalizeAngle(int *angle);

  // Modern OpenGL Resources (Qt6)
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo;
  std::unique_ptr<QOpenGLShaderProgram> program;
  QOpenGLTexture *texture;

  // Transform Matrices
  QMatrix4x4 projection;

  // Vertex count for drawing
  int vertexCount;

  struct DrawCommand {
    int offset;
    int count;
    QOpenGLTexture *texture;
    bool isGlass;
  };
  std::vector<DrawCommand> drawCommands;

  // Synchronization flags for thread/GL race condition
  bool objectsLoaded;
  bool glInitialized;

  float xRot;
  float yRot;
  float zRot;
  float xTrans;
  float yTrans;
  float zTrans;

  QColor trolltechPurple;

  std::unique_ptr<ReaderThread> rt;
};

#endif

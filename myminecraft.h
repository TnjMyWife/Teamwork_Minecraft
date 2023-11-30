#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <qgl.h>      
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QVector3D>
#include <QMouseEvent>
#include <QtMath>
#include <vector>
#include "Camera/Camera.h"
#include "Character/Character.h"
#include "Collision/Collision.h"
#include "CubeAndChunk/chunkList.h"

class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget* parent = 0, bool fs = false);
    ~MyGLWidget();


protected:
    void initializeGL() override;     
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    Cube* createCube(int cubeType);

private:
    bool fullscreen;    // 是否处于全屏状态
    bool firstClick;
    float cubeSize;
    float yaw;
    float pitch;
    QTimer* timer;

    Camera camera;
    Character character;
    Collision collision;
};

#endif // MYGLWIDGET_H



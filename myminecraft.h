#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <qgl.h>      
#include <QKeyEvent>
#include <QTimer>
#include <QVector3D>
#include <QMouseEvent>
#include <QtMath>
#include <vector>
#include <Camera.h>
#include <Character.h>
#include <Collision.h>


class MyGLWidget : public QGLWidget
{
public:
    MyGLWidget(QWidget* parent = 0, bool fs = false);
    ~MyGLWidget();

protected:
    void initializeGL() override;           //用来初始化OPenGL窗口，可以在里面设定一些有关选项
    void paintGL() override;                //用来绘制OPenGL的窗口，只要有更新发生，这个函数就会被调用
    void resizeGL(int w, int h) override;   //用来处理窗口大小变换这一事件，resizeGL()在处理完后会自动刷新屏幕

    void keyPressEvent(QKeyEvent* e) override;  //Qt键盘事件处理函数
    void keyReleaseEvent(QKeyEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void loadGLTextures();  //载入指定的图片并生成相应的纹理
    void drawGrassCube(float x, float y, float z);
    void drawPlain();

private:
    bool fullscreen;    // 用来保存窗口是否处于全屏状态的变量
    bool firstClick;
    float cubeSize;
    float yaw;
    float pitch;
    QTimer* timer;
    GLuint texture[3];  //用来存储纹理（长度为1的数组）

    Camera camera;
    Character character;
    Collision collision;
};

#endif // MYGLWIDGET_H



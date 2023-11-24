#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <qgl.h>        //因为QGLWidget类被包含在qgl.h头文件中
#include <QKeyEvent>
#include <QTimer>
#include <QVector3D>
#include <QMouseEvent>
#include <QtMath>

//继承QGLWidget得到OPenGL窗口部件类
class MyGLWidget : public QGLWidget
{
public:
    MyGLWidget(QWidget* parent = 0, bool fs = false);
    ~MyGLWidget();

protected:
    /*
    QGLWidget 类已经内置了对 OpenGL 的处理，就是通过对 initializeGL()、 paintGL()和 resizeGL()这个三个函数实现
    */
    void initializeGL() override;           //用来初始化OPenGL窗口，可以在里面设定一些有关选项
    void paintGL() override;                //用来绘制OPenGL的窗口，只要有更新发生，这个函数就会被调用
    void resizeGL(int w, int h) override;   //用来处理窗口大小变换这一事件，resizeGL()在处理完后会自动刷新屏幕

    void keyPressEvent(QKeyEvent* e) override;  //Qt键盘事件处理函数
    void mouseMoveEvent(QMouseEvent* e) override;
    void loadGLTextures();  //载入指定的图片并生成相应的纹理
    void drawGrassCube(float x, float y, float z);
    void drawPlain();

    /* 人物模型部分 */
    void drawHead();
    void drawBody();
    void drawOne();
    void drawArm();
    void drawLeg();
    void drawCharacter();

private:
    bool fullscreen;    //用来保存窗口是否处于全屏状态的变量
    bool firstClick;
    float cubeSize;
    float yaw;
    float pitch;
    float cameraSpeed;
    QTimer* timer;
    GLuint texture[3];  //用来存储纹理（长度为1的数组）
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

    void handleCamera();
    void updateCameraVectors();
};

#endif // MYGLWIDGET_H



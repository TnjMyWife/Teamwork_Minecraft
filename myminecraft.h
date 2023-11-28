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

struct object {
    QVector3D leftbottom;
    QVector3D rightup;
    object(QVector3D vector1, QVector3D vector2) : leftbottom(vector1), rightup(vector2) {}
};

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

    /* 人物模型 */
    void drawHead();
    void drawBody();
    void drawOne();
    void drawArm();
    void drawLeg();
    void drawCharacter();

    /* 碰撞检测 */
    bool iscollision(QVector3D temp, float x0, float y0, float z0, float x1, float y1, float z1);
    bool twoRectanglescollision(float x1, float y1, float z1, float x2, float y2, float z2,
        float x3, float y3, float z3, float x4, float y4, float z4);
    bool allcollision(QVector3D temp);

private:
    bool fullscreen;    // 用来保存窗口是否处于全屏状态的变量
    bool firstClick;
    bool firstperspect;
    float cubeSize;
    float yaw;
    float pitch;
    float cameraSpeed;
    float angle;
    float swingSpeed;
    std::vector<object> myobject;
    QTimer* timer;
    GLuint texture[3];  //用来存储纹理（长度为1的数组）
    QVector3D cameraPos = QVector3D(0.0f, 0.28f, 0.0f);     // 相机位置，第一人称需要抬至头部位置，初始化为(0.0f, 0.28f, 0.0f)，
                                                                //第三人称：需要将相机抬得比0.28高，并在人物后方(0.0f, 0.4f, -1.0f)，方向沿Z正方向
    QVector3D characterPos = QVector3D(0.0f, 0.0f, 0.0f);   // 人物相对初始头部(0.0, 0.28, 0.0)移动的位置,不用改动
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

    void handleCamera();
    void updateCameraVectors();
    void handleSwing();         // 手臂摆动

};

#endif // MYGLWIDGET_H



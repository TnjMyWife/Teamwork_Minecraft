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
#include <QString>
#include <QOpenGLWidget>
#include <QVector>
enum blockType { GRASS };
enum direction { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };
class Chunk
{
private:
    QVector<QVector<QVector<int>>> map;// 存储chunk内部每个位置的方块类型
    QVector3D pos;// 绘制起始位置
    int chunkSize;

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// 检查chunk内(x, y, z)位置的方块是否存在，若存在，相邻位置的方块不用绘制表面
    void buildChunk();
    void setMap(QVector<QVector<QVector<int>>> mp);
    void setPos(QVector3D ps);
    void setChunkSize(int size);
    
};

class Cube // 方块类，所有方块的抽象；变量：方块的大小、绑定的材质、硬度等；方法：绘制方块等
{
protected:
    float cubeSize;
    GLuint texture[3];//top, side, bottom
    bool isVisible[6];// 前后左右上下

public:
    Cube(float size, QStringList Images);
    ~Cube();
    void drawCube(float x, float y, float z);
    void Cube::setInvisible(int i);
    void Cube::resetVisible();
    float getCubeSize();

};

//继承QGLWidget得到OPenGL窗口部件类
class MyGLWidget : public QOpenGLWidget
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

    /* 人物模型 */
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
    float angle;
    float swingSpeed;
    QTimer* timer;
    GLuint texture[3];  //用来存储纹理（长度为1的数组）
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

    void handleCamera();
    void updateCameraVectors();
    void MyGLWidget::handleSwing();
};

static QVector<Cube*> cubeList(0);

#endif // MYGLWIDGET_H



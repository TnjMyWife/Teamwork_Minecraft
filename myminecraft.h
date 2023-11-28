#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <qgl.h>        //��ΪQGLWidget�౻������qgl.hͷ�ļ���
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
    QVector<QVector<QVector<int>>> map;// �洢chunk�ڲ�ÿ��λ�õķ�������
    QVector3D pos;// ������ʼλ��
    int chunkSize;

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// ���chunk��(x, y, z)λ�õķ����Ƿ���ڣ������ڣ�����λ�õķ��鲻�û��Ʊ���
    void buildChunk();
    void setMap(QVector<QVector<QVector<int>>> mp);
    void setPos(QVector3D ps);
    void setChunkSize(int size);
    
};

class Cube // �����࣬���з���ĳ��󣻱���������Ĵ�С���󶨵Ĳ��ʡ�Ӳ�ȵȣ����������Ʒ����
{
protected:
    float cubeSize;
    GLuint texture[3];//top, side, bottom
    bool isVisible[6];// ǰ����������

public:
    Cube(float size, QStringList Images);
    ~Cube();
    void drawCube(float x, float y, float z);
    void Cube::setInvisible(int i);
    void Cube::resetVisible();
    float getCubeSize();

};

//�̳�QGLWidget�õ�OPenGL���ڲ�����
class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget* parent = 0, bool fs = false);
    ~MyGLWidget();

protected:
    /*
    QGLWidget ���Ѿ������˶� OpenGL �Ĵ�������ͨ���� initializeGL()�� paintGL()�� resizeGL()�����������ʵ��
    */
    void initializeGL() override;           //������ʼ��OPenGL���ڣ������������趨һЩ�й�ѡ��
    void paintGL() override;                //��������OPenGL�Ĵ��ڣ�ֻҪ�и��·�������������ͻᱻ����
    void resizeGL(int w, int h) override;   //���������ڴ�С�任��һ�¼���resizeGL()�ڴ��������Զ�ˢ����Ļ

    void keyPressEvent(QKeyEvent* e) override;  //Qt�����¼�������
    void mouseMoveEvent(QMouseEvent* e) override;
    void loadGLTextures();  //����ָ����ͼƬ��������Ӧ������
    void drawGrassCube(float x, float y, float z);
    void drawPlain();

    /* ����ģ�� */
    void drawHead();
    void drawBody();
    void drawOne();
    void drawArm();
    void drawLeg();
    void drawCharacter();

private:
    bool fullscreen;    //�������洰���Ƿ���ȫ��״̬�ı���
    bool firstClick;
    float cubeSize;
    float yaw;
    float pitch;
    float cameraSpeed;
    float angle;
    float swingSpeed;
    QTimer* timer;
    GLuint texture[3];  //�����洢��������Ϊ1�����飩
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

    void handleCamera();
    void updateCameraVectors();
    void MyGLWidget::handleSwing();
};

static QVector<Cube*> cubeList(0);

#endif // MYGLWIDGET_H



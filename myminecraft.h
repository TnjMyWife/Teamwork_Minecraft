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

//�̳�QGLWidget�õ�OPenGL���ڲ�����
class MyGLWidget : public QGLWidget
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

    /* ����ģ�Ͳ��� */
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
    QTimer* timer;
    GLuint texture[3];  //�����洢��������Ϊ1�����飩
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

    void handleCamera();
    void updateCameraVectors();
};

#endif // MYGLWIDGET_H



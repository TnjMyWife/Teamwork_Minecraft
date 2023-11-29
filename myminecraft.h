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
    void initializeGL() override;           //������ʼ��OPenGL���ڣ������������趨һЩ�й�ѡ��
    void paintGL() override;                //��������OPenGL�Ĵ��ڣ�ֻҪ�и��·�������������ͻᱻ����
    void resizeGL(int w, int h) override;   //���������ڴ�С�任��һ�¼���resizeGL()�ڴ��������Զ�ˢ����Ļ

    void keyPressEvent(QKeyEvent* e) override;  //Qt�����¼�������
    void keyReleaseEvent(QKeyEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void loadGLTextures();  //����ָ����ͼƬ��������Ӧ������
    void drawGrassCube(float x, float y, float z);
    void drawPlain();

private:
    bool fullscreen;    // �������洰���Ƿ���ȫ��״̬�ı���
    bool firstClick;
    float cubeSize;
    float yaw;
    float pitch;
    QTimer* timer;
    GLuint texture[3];  //�����洢��������Ϊ1�����飩

    Camera camera;
    Character character;
    Collision collision;
};

#endif // MYGLWIDGET_H



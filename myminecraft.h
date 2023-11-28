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
    void initializeGL() override;           //������ʼ��OPenGL���ڣ������������趨һЩ�й�ѡ��
    void paintGL() override;                //��������OPenGL�Ĵ��ڣ�ֻҪ�и��·�������������ͻᱻ����
    void resizeGL(int w, int h) override;   //���������ڴ�С�任��һ�¼���resizeGL()�ڴ��������Զ�ˢ����Ļ

    void keyPressEvent(QKeyEvent* e) override;  //Qt�����¼�������
    void keyReleaseEvent(QKeyEvent* e) override;
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

    /* ��ײ��� */
    bool iscollision(QVector3D temp, float x0, float y0, float z0, float x1, float y1, float z1);
    bool twoRectanglescollision(float x1, float y1, float z1, float x2, float y2, float z2,
        float x3, float y3, float z3, float x4, float y4, float z4);
    bool allcollision(QVector3D temp);

private:
    bool fullscreen;    // �������洰���Ƿ���ȫ��״̬�ı���
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
    GLuint texture[3];  //�����洢��������Ϊ1�����飩
    QVector3D cameraPos = QVector3D(0.0f, 0.28f, 0.0f);     // ���λ�ã���һ�˳���Ҫ̧��ͷ��λ�ã���ʼ��Ϊ(0.0f, 0.28f, 0.0f)��
                                                                //�����˳ƣ���Ҫ�����̧�ñ�0.28�ߣ����������(0.0f, 0.4f, -1.0f)��������Z������
    QVector3D characterPos = QVector3D(0.0f, 0.0f, 0.0f);   // ������Գ�ʼͷ��(0.0, 0.28, 0.0)�ƶ���λ��,���øĶ�
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

    void handleCamera();
    void updateCameraVectors();
    void handleSwing();         // �ֱ۰ڶ�

};

#endif // MYGLWIDGET_H



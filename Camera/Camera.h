#pragma once
#include <QVector3D>
#include <GL/glew.h>
#include <QtMath>
#include <qmatrix4x4.h>
class Camera
{
public:
    Camera();
    ~Camera();

    QVector3D getCameraPos() const;
    QVector3D getCharacterPos() const;
    QVector3D getCameraFront() const;
    QVector3D getCameraUp() const;
    QMatrix4x4 getVewMat();

    void turnPerspect();                    // α�����˳�
    void updateCameraVectors(float yaw, float pitch);       // ����ƶ��������
    void moveForward();       // ǰ������
    void moveBack();          // ���˲���
    void moveLeft();          // ����
    void moveRight();         // ����
    void moveDown();          // ����(����)
    void moveUp();


    void gravity();
private:
    bool firstPerspect;
	float cameraSpeed;
    QVector3D velocity;
    QVector3D cameraPos;

    QVector3D targetPos;

    QVector3D characterPos;
    QVector3D cameraFront;
    QVector3D cameraUp;
};

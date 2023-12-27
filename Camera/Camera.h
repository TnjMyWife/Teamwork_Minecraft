#pragma once
#include <QVector3D>
#include <GL/glew.h>
#include <QtMath>
#include <qmatrix4x4.h>
#include "../Collision/Collision.h"
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

    void turnPerspect();                    // ��������õ�����
    void updateCameraVectors(float yaw, float pitch);       // ����ƶ������������
    void moveForward(const Collision& c);       // ǰ������
    void moveBack(const Collision& c);          // ���˲���
    void moveLeft(const Collision& c);          // ����
    void moveRight(const Collision& c);         // ����
    void moveDown(const Collision& c);          // ����(����)
    void moveUp(const Collision& c);            // ��Ծ

    void resetVel();            // ���ô�ֱ�ٶ�
    void gravity(const Collision& c);           // �������µĴ�ֱ����λ�õĸ���
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

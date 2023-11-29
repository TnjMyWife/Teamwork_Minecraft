#pragma once
#include <QVector3D>
#include <GL/glew.h>
#include <QtMath>
#include <Collision.h>

class Camera
{
public:
    Camera();
    ~Camera();

    QVector3D getCameraPos() const;
    QVector3D getCharacterPos() const;
    QVector3D getCameraFront() const;
    void turnPerspect();
    void updateCameraVectors(float yaw, float pitch);
    void setCamera();

    void moveForward(const Collision& c);
    void moveBack(const Collision& c);
    void moveLeft(const Collision& c);
    void moveRight(const Collision& c);

private:
    bool firstPerspect;
	float cameraSpeed;
    QVector3D cameraPos = QVector3D(0.0f, 0.28f, 0.0f);     // ���λ�ã���һ�˳���Ҫ̧��ͷ��λ�ã���ʼ��Ϊ(0.0f, 0.28f, 0.0f)��
    //�����˳ƣ���Ҫ�����̧�ñ�0.28�ߣ����������(0.0f, 0.4f, -1.0f)��������Z������
    QVector3D characterPos = QVector3D(0.0f, 0.0f, 0.0f);   // ������Գ�ʼͷ��(0.0, 0.28, 0.0)�ƶ���λ��,���øĶ�
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
};

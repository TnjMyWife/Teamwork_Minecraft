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
    QVector3D cameraPos = QVector3D(0.0f, 0.28f, 0.0f);     // 相机位置，第一人称需要抬至头部位置，初始化为(0.0f, 0.28f, 0.0f)，
    //第三人称：需要将相机抬得比0.28高，并在人物后方(0.0f, 0.4f, -1.0f)，方向沿Z正方向
    QVector3D characterPos = QVector3D(0.0f, 0.0f, 0.0f);   // 人物相对初始头部(0.0, 0.28, 0.0)移动的位置,不用改动
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
};

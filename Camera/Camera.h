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

    void turnPerspect();                    // 伪第三人称
    void updateCameraVectors(float yaw, float pitch);       // 鼠标移动更新相机
    void moveForward();       // 前进操作
    void moveBack();          // 后退操作
    void moveLeft();          // 向左
    void moveRight();         // 向右
    void moveDown();          // 向下(蹲下)
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

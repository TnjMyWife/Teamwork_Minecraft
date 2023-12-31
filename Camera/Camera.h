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

    void turnPerspect();                    // 将相机放置到正面
    void updateCameraVectors(float yaw, float pitch);       // 鼠标移动更新相机参数
    void moveForward(const Collision& c);       // 前进操作
    void moveBack(const Collision& c);          // 后退操作
    void moveLeft(const Collision& c);          // 向左
    void moveRight(const Collision& c);         // 向右
    void moveDown(const Collision& c);          // 向下(蹲下)
    void moveUp(const Collision& c);            // 跳跃

    void resetVel();            // 重置垂直速度
    void gravity(const Collision& c);           // 重力导致的垂直方向位置的更新
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

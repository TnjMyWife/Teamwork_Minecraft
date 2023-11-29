#pragma once
#include<QVector3D>
#include <vector>
//用来存储创建的物体，因为物体多成长方体，所以只需要记录左上角和右下角即可。
struct object {
    QVector3D leftbottom;
    QVector3D rightup;
    object(QVector3D vector1, QVector3D vector2) : leftbottom(vector1), rightup(vector2) {}
};

class Collision
{
public:
	Collision();
	~Collision();
    //基于相机视角的碰撞检测（基本没用）
    bool isCollision(QVector3D temp, float x0, float y0, float z0, float x1, float y1, float z1) const;
    //两个方块之间的碰撞检测
    bool twoRectanglescollision(QVector3D leftbottom1, QVector3D rightup1, QVector3D leftbottom2, QVector3D rightup2) const;
    bool allcollision(QVector3D temp) const;//对每一个物体进行碰撞检测
    std::vector<object> objects;//用一个数组来存储创建的物体
};

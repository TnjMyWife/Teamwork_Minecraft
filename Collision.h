#pragma once
#include<QVector3D>
#include <vector>

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

    bool isCollision(QVector3D temp, float x0, float y0, float z0, float x1, float y1, float z1) const;
    bool twoRectanglescollision(float x1, float y1, float z1, float x2, float y2, float z2,
        float x3, float y3, float z3, float x4, float y4, float z4) const;
    bool allcollision(QVector3D temp) const;
    std::vector<object> objects;
};

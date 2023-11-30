#include<Collision.h>

Collision::Collision()
{

}

Collision::~Collision()
{

}

bool Collision::isCollision(QVector3D temp, float x0, float y0, float z0, float x1, float y1, float z1) const{
	float x2 = temp.x(), y2 = temp.y(), z2 = temp.z();
	// 检查 x2 是否在 x0 和 x1 之间
	bool withinX = (x2 >= x0 - 0.041) && (x2 <= x1 + 0.041);

	// 检查 y2 是否在 y0 和 y1 之间
	bool withinY = (y2 >= y0 - 0.041) && (y2 <= y1 + 0.041);

	// 检查 z2 是否在 z0 和 z1 之间
	bool withinZ = (z2 >= z0 - 0.041) && (z2 <= z1 + 0.041);

	// 如果 x2、y2、z2 都在范围内，则点在长方体内
	return withinX && withinY && withinZ;
}


bool Collision::twoRectanglescollision(float x1, float y1, float z1, float x2, float y2, float z2,
	float x3, float y3, float z3, float x4, float y4, float z4) const {
	// Check if one rectangle is to the left of the other
	if (x2 < x3 || x4 < x1) {
		return false;
	}

	// Check if one rectangle is above the other
	if (y2 < y3 || y4 < y1) {
		return false;
	}

	// Check if one rectangle is behind the other
	if (z2 < z3 || z4 < z1) {
		return false;
	}

	// If the above conditions are not met, the rectangles must be intersecting
	return true;
}

bool Collision::allcollision(QVector3D temp) const {
	for (int i = 0; i < objects.size(); i++) {
		QVector3D v1 = objects[i].leftbottom;
		QVector3D v2 = objects[i].rightup;
		float x1 = v1.x(), y1 = v1.y(), z1 = v1.z();
		float x2 = v2.x(), y2 = v2.y(), z2 = v2.z();
		if (isCollision(temp, x1, y1, z1, x2, y2, z2))
			return true;
	}
	return false;
}
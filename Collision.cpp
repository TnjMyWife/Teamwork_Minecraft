#include<Collision.h>

Collision::Collision()
{

}

Collision::~Collision()
{

}

bool Collision::isCollision(QVector3D temp, float x0, float y0, float z0, float x1, float y1, float z1) const{
	float x2 = temp.x(), y2 = temp.y(), z2 = temp.z();
	// ��� x2 �Ƿ��� x0 �� x1 ֮��
	bool withinX = (x2 >= x0 - 0.041) && (x2 <= x1 + 0.041);

	// ��� y2 �Ƿ��� y0 �� y1 ֮��
	bool withinY = (y2 >= y0 - 0.041) && (y2 <= y1 + 0.041);

	// ��� z2 �Ƿ��� z0 �� z1 ֮��
	bool withinZ = (z2 >= z0 - 0.041) && (z2 <= z1 + 0.041);

	// ��� x2��y2��z2 ���ڷ�Χ�ڣ�����ڳ�������
	return withinX && withinY && withinZ;
}

bool Collision::twoRectanglescollision(QVector3D leftbottom1, QVector3D rightup1, QVector3D leftbottom2, QVector3D rightup2) const{
	float x1 = leftbottom1.x(), y1 = leftbottom1.y(), z1 = leftbottom1.z();
	float x2 = rightup1.x(), y2 = rightup1.y(), z2 = rightup1.z();
	float x3 = leftbottom2.x(), y3 = leftbottom2.y(), z3 = leftbottom2.z();
	float x4 = rightup2.x(), y4 = rightup2.y(), z4 = rightup2.z();
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
	//��һ�����������ס������õ�����������½Ǻ����Ͻǡ�
	QVector3D body1 = temp - QVector3D(0.04, 0.28, 0.04);
	QVector3D body2 = temp + QVector3D(0.04, 0.04, 0.04);
	//�Դ洢ÿһ�����������ײ���
	for (int i = 0; i < objects.size(); i++) {
		//��ȡҪ�����������½Ǻ����Ͻ�
		QVector3D v1 = objects[i].leftbottom;
		QVector3D v2 = objects[i].rightup;
		float x1 = v1.x(), y1 = v1.y(), z1 = v1.z();
		float x2 = v2.x(), y2 = v2.y(), z2 = v2.z();
		/*
		if (iscollision(temp, x1, y1, z1, x2, y2, z2))//�����ײ��⣬�������á�
			return true;
		*/
		if (twoRectanglescollision(body1, body2, v1, v2))
			return true;
	}
	return false;
}
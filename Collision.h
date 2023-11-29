#pragma once
#include<QVector3D>
#include <vector>
//�����洢���������壬��Ϊ�����ɳ����壬����ֻ��Ҫ��¼���ϽǺ����½Ǽ��ɡ�
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
    //��������ӽǵ���ײ��⣨����û�ã�
    bool isCollision(QVector3D temp, float x0, float y0, float z0, float x1, float y1, float z1) const;
    //��������֮�����ײ���
    bool twoRectanglescollision(QVector3D leftbottom1, QVector3D rightup1, QVector3D leftbottom2, QVector3D rightup2) const;
    bool allcollision(QVector3D temp) const;//��ÿһ�����������ײ���
    std::vector<object> objects;//��һ���������洢����������
};

#pragma once
#include <QVector3D>
#include <QHash>
//�����洢���������壬��Ϊ�����ɳ����壬����ֻ��Ҫ��¼���ϽǺ����½Ǽ��ɡ�
struct object {
    QVector3D leftbottom;
    QVector3D rightup;
    object(QVector3D vector1, QVector3D vector2) : leftbottom(vector1), rightup(vector2) {}
};



extern QVector<object> objects;
extern QHash <QString, bool> deleted_object;
extern QVector<QVector3D> added_object;
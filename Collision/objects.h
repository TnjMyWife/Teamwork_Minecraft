#pragma once
#include <QVector3D>
#include <QHash>
//用来存储创建的物体，因为物体多成长方体，所以只需要记录左上角和右下角即可。
struct object {
    QVector3D leftbottom;
    QVector3D rightup;
    object(QVector3D vector1, QVector3D vector2) : leftbottom(vector1), rightup(vector2) {}
};



extern QVector<object> objects;
extern QHash <QString, bool> deleted_object;
extern QVector<QVector3D> added_object;
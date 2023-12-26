#include "objects.h"


inline uint qHash(const QVector3D& key) {
    // 使用 qHash 函数计算 QVector3D 对象的哈希值
    return qHash(QVector3D::dotProduct(key, QVector3D(73856093, 19349663, 83492791)));
}


QVector<object> objects = QVector<object>();
QVector<QVector3D> added_object;
QHash <QString, bool> deleted_object;
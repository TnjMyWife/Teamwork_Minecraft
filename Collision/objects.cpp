#include "objects.h"


inline uint qHash(const QVector3D& key) {
    // ʹ�� qHash �������� QVector3D ����Ĺ�ϣֵ
    return qHash(QVector3D::dotProduct(key, QVector3D(73856093, 19349663, 83492791)));
}


QVector<object> objects = QVector<object>();
QVector<QVector3D> added_object;
QHash <QString, bool> deleted_object;
#include "change_cube.h"

// added_object �� deleted_object �ڡ�object.h������������
change_cube::change_cube(){}
change_cube::~change_cube() {}
//������ӳ�䵽������
QVector3D change_cube::get_relative_position(QVector3D vector, float h) {
	int x = static_cast<int>(vector.x() / (2 * h));
	x = (vector.x() > 0) ? x : (x - 1);
	int y = static_cast<int>(vector.y() / (2 * h));
	y = (vector.y() > 0) ? y : (y - 1);
	int z = static_cast<int>(vector.z() / (2 * h));
	z = (vector.z() > 0) ? z : (z - 1);
	return QVector3D(x, y, z);
}

void change_cube::tochange_cube(Cube* cube, Camera* camera, Collision& collision, bool action, int args) {

	float h = cube->getCubeSize();
	QVector3D pos = camera->getCameraPos();
	QVector3D front = camera->getCameraFront();
	//front = QVector3D(front.x(), 0, front.z());
	//front.normalize();
	//ƫ�����ļ���
	float axis = 0.3 + args * 0.12; 
	//��ӻ�ɾ�������λ�ã������λ�� + ����ķ��� *  һ����ƫ����
	QVector3D add_pos = QVector3D(pos.x() + front.x() * axis, pos.y() + front.y() * axis, pos.z() + front.z() * axis);
	QVector3D add = get_relative_position(add_pos, h);
	int i = add.x(), j = add.y(), k = add.z();
	//���������ż���ʵ����ӷ����λ��
	QVector3D center = QVector3D((2 * i + 1) * h, (2 * j + 1) * h, (2 * k + 1) * h);
	//��ʵ��λ��ת��Ϊ�ַ������Ա�����ɾ������ʱ�Ĺ�ϣӳ��
	QString result = QString("X: %1, Y: %2, Z: %3")
		.arg(center.x())
		.arg(center.y())
		.arg(center.z());
	if (action == ADD) {	
		QVector3D body1 = pos - QVector3D(0.04, 0.28, 0.04);
		QVector3D body2 = pos + QVector3D(0.04, 0.04, 0.04);
		QVector3D leftbottom = center - QVector3D(h, h, h);
		QVector3D rightup = center + QVector3D(h, h, h);
		//�����ӵ�λ���Ƿ����Լ������巢����ײ
		if (!collision.twoRectanglescollision(body1, body2, leftbottom, rightup)) {
			added_object.push_back(center);
		}
		//�����鱻���ó�ɾ��״̬
		if (deleted_object[result])
			deleted_object[result] = false;
	}
	else if (action == DELETE) {
		//���������ó�ɾ��״̬
		deleted_object[result] = true;
	}
	return;	
}









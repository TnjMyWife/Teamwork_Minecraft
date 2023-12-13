#include "change_cube.h"

// added_object 和 deleted_object 在“object.h”中声明定义
change_cube::change_cube(){}
change_cube::~change_cube() {}
//将坐标映射到网格编号
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
	//偏移量的计算
	float axis = 0.3 + args * 0.12; 
	//添加或删除方块的位置：相机的位置 + 相机的方向 *  一定的偏移量
	QVector3D add_pos = QVector3D(pos.x() + front.x() * axis, pos.y() + front.y() * axis, pos.z() + front.z() * axis);
	QVector3D add = get_relative_position(add_pos, h);
	int i = add.x(), j = add.y(), k = add.z();
	//根据网格编号计算实际添加方块的位置
	QVector3D center = QVector3D((2 * i + 1) * h, (2 * j + 1) * h, (2 * k + 1) * h);
	//将实际位置转化为字符串，以便于做删除方块时的哈希映射
	QString result = QString("X: %1, Y: %2, Z: %3")
		.arg(center.x())
		.arg(center.y())
		.arg(center.z());
	if (action == ADD) {	
		QVector3D body1 = pos - QVector3D(0.04, 0.28, 0.04);
		QVector3D body2 = pos + QVector3D(0.04, 0.04, 0.04);
		QVector3D leftbottom = center - QVector3D(h, h, h);
		QVector3D rightup = center + QVector3D(h, h, h);
		//检测添加的位置是否与自己的身体发送碰撞
		if (!collision.twoRectanglescollision(body1, body2, leftbottom, rightup)) {
			added_object.push_back(center);
		}
		//若方块被设置成删除状态
		if (deleted_object[result])
			deleted_object[result] = false;
	}
	else if (action == DELETE) {
		//将方块设置成删除状态
		deleted_object[result] = true;
	}
	return;	
}









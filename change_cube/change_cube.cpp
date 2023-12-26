#include "change_cube.h"

// added_object 和 deleted_object 在“object.h”中声明定义
change_cube::change_cube() {}
change_cube::~change_cube() {}
//将坐标映射到chunk内部的网格编号
QVector3D change_cube::get_relative_position(QVector3D vector, float h, QVector3D base) {
	int x = std::round((vector.x() - base.x()) / (2 * h));

	int y = std::round((vector.y() - base.y()) / (2 * h));

	int z = std::round((vector.z() - base.z()) / (2 * h));

	return QVector3D(x, y, z);
}

void change_cube::tochange_cube(QMatrix4x4 m_projectMat, QOpenGLShaderProgram*  m_program1, Camera* camera, Collision& collision, bool action, int args) {

	float h = cubeList[0]->getCubeSize();
	QVector3D pos = camera->getCameraPos();
	QVector3D front = camera->getCameraFront();
	//front = QVector3D(front.x(), 0, front.z());
	//front.normalize();
	//偏移量的计算
	float axis = 0.5 + args * 0.12;
	//添加或删除方块的位置：相机的位置 + 相机的方向 *  一定的偏移量
	QVector3D add_pos = QVector3D(pos.x() + front.x() * axis, pos.y() + front.y() * axis, pos.z() + front.z() * axis);
	// 确定区块位置
	float chunk_x = 0.0;
	float chunk_y = 0.0;
	float chunk_z = 0.0;
	int id = 0;
	for (int i = 0; i < chunkList.length(); ++i) {
		QVector3D chunk_pos = chunkList.at(i)->getPos();
		int chunk_size = chunkList.at(i)->getChunkSize();
		if (add_pos.x() >= chunk_pos.x() - h &&
			add_pos.x() < chunk_pos.x() - h + chunk_size * h * 2 &&
			add_pos.z() >= chunk_pos.z() - h &&
			add_pos.z() < chunk_pos.z() - h + chunk_size * h * 2) {
			chunk_x = chunk_pos.x();
			chunk_y = chunk_pos.y();
			chunk_z = chunk_pos.z();
			id = i;
			break;
		}
	}
	QVector3D selected_chunk_pos = QVector3D(chunk_x, chunk_y, chunk_z);
	QVector3D add = get_relative_position(add_pos, h, selected_chunk_pos);
	/*
	int i = add.x(), j = add.y(), k = add.z();
	//根据网格编号计算实际添加方块的位置
	QVector3D center = QVector3D((2 * i + 1) * h, (2 * j + 1) * h, (2 * k + 1) * h);
	//将实际位置转化为字符串，以便于做删除方块时的哈希映射
	QString result = QString("X: %1, Y: %2, Z: %3")
		.arg(center.x())
		.arg(center.y())
		.arg(center.z());
	*/
	if (add.x() < 0 ||
		add.y() < 0 ||
		add.z() < 0 ||
		add.x() >= chunkList[0]->getChunkSize() ||
		add.y() >= chunkList[0]->getChunkSize() ||
		add.z() >= chunkList[0]->getChunkSize())
		return;
	if (action == ADD) {
		/*
		QVector3D body1 = pos - QVector3D(0.04, 0.28, 0.04);
		QVector3D body2 = pos + QVector3D(0.04, 0.04, 0.04);
		QVector3D leftbottom = center - QVector3D(h, h, h);
		QVector3D rightup = center + QVector3D(h, h, h);
		//检测添加的位置是否与自己的身体发送碰撞
		if (!collision.twoRectanglescollision(body1, body2, leftbottom, rightup)) {
			//added_object.push_back(center);
		}
		//若方块被设置成删除状态
		if (deleted_object[result])
			deleted_object[result] = false;
		*/
		chunkList[id]->addCube(add, m_projectMat, camera->getVewMat(), m_program1, 0);
	}
	else if (action == DELETE) {
		//将方块设置成删除状态
		//deleted_object[result] = true;
		chunkList[id]->deleteCube(add, m_projectMat, camera->getVewMat(), m_program1);
	}
	return;
}



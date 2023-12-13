#include "Chunk.h"

Chunk::Chunk() {
	this->chunkSize = 30;
	this->map = QVector<QVector<QVector<int>>>(chunkSize, QVector<QVector<int>>(chunkSize, QVector<int>(chunkSize, 0)));
	this->pos = QVector3D(0.0f, 0.0f, 0.0f);

}

Chunk::~Chunk() {

}

bool Chunk::isCubeExist(int x, int y, int z) {
	// chunk外视为没有方块
	if (x < 0 || y < 0 || z < 0 || x >= chunkSize || y >= chunkSize || z >= chunkSize) return false;
	if (map[x][y][z] == -1) {
		return false;
	}
	else return true;
}

void Chunk::buildChunk() {
	float x = pos.x(), y = pos.y(), z = pos.z();
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (map[i][j][k] >= 0) {// 对应x, y, z
					Cube* tmp = cubeList[map[i][j][k]];
					tmp->resetVisible();
					float spacing = tmp->getCubeSize() * 2;
					if (isCubeExist(i, j + 1, k)) {
						tmp->setInvisible(TOP);
					}
					if (isCubeExist(i, j - 1, k)) {
						tmp->setInvisible(BOTTOM);
					}
					if (isCubeExist(i - 1, j, k)) {
						tmp->setInvisible(LEFT);
					}
					if (isCubeExist(i + 1, j, k)) {
						tmp->setInvisible(RIGHT);
					}
					if (isCubeExist(i, j, k + 1)) {
						tmp->setInvisible(FRONT);
					}
					if (isCubeExist(i, j, k - 1)) {
						tmp->setInvisible(BACK);
					}
					tmp->drawCube(x + i * spacing, y + j * spacing, z + k * spacing);
					tmp->resetVisible();
				}
			}
		}
	}
}

void Chunk::setMap() {
	//stone*5, dirt*5, grass*1
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (j < 5) {
					map[i][j][k] = STONE;
				}
				else if (j < 10) {
					map[i][j][k] = DIRT;
				}
				else if (j < 11) {
					map[i][j][k] = GRASS;
				}
				else {
					map[i][j][k] = -1;
				}
			}
		}
	}
}
void Chunk::setPos(QVector3D ps) {
	this->pos.setX(ps.x());
	this->pos.setY(ps.y());
	this->pos.setZ(ps.z());

}
void Chunk::setChunkSize(int size) {
	this->chunkSize = size;
}
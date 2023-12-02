#include "Chunk.h"

Chunk::Chunk() {
	this->chunkSize = 24;
	this->map = QVector<QVector<QVector<int>>>(chunkSize, QVector<QVector<int>>(chunkSize, QVector<int>(chunkSize, 0)));
	this->pos = QVector3D(0.0f, 0.0f, 0.0f);
	this->visible_info = vector<vector<vector<vector<bool>>>>(chunkSize, vector<vector<vector<bool>>>(chunkSize, vector<vector<bool>>(chunkSize, vector<bool>(6, true))));
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
					tmp->setVisible(visible_info[i][j][k]);
					tmp->drawCube(x + i * spacing, y + j * spacing, z + k * spacing);
					tmp->resetVisible();
				}
			}
		}
	}
}

void Chunk::setMap() {
	//stone*5, dirt*5, grass*1
#pragma omp parallel for collapse(3)
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
		//printf("i=%d thread:%d\n", i, omp_get_thread_num());
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

void Chunk::calculateVisible() {
#pragma omp parallel for collapse(3)
	for (int i = 0; i < chunkSize; ++i) {
		//int thread = omp_get_thread_num();
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (map[i][j][k] != -1) {
					if (isCubeExist(i - 1, j, k)) {// 若左边的物体存在，那么该位置物体左面不可视，同时左边的物体右面不可视
						visible_info[i][j][k][LEFT] = false;
						visible_info[i - 1][j][k][RIGHT] = false;
					}
					if (isCubeExist(i, j - 1, k)) {// 下面
						visible_info[i][j][k][BOTTOM] = false;
						visible_info[i][j - 1][k][TOP] = false;
					}
					if (isCubeExist(i, j, k - 1)) {// 后面
						visible_info[i][j][k][BACK] = false;
						visible_info[i][j][k - 1][FRONT] = false;
					}
					
				}
			}
		}
		//printf("i=%d, thread:%d\n", i, omp_get_thread_num());
	}
}

int Chunk::getChunkSize() {
	return chunkSize;
}
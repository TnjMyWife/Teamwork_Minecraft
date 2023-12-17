#include "Chunk.h"

Chunk::Chunk() {
	this->chunkSize = 8;
	this->map = QVector<QVector<QVector<int>>>(chunkSize, QVector<QVector<int>>(chunkSize, QVector<int>(chunkSize, 0)));
	this->pos = QVector3D(0.0f, 0.0f, 0.0f);
	this->visible_info = vector<vector<vector<vector<bool>>>>(chunkSize, vector<vector<vector<bool>>>(chunkSize, vector<vector<bool>>(chunkSize, vector<bool>(6, true))));
	this->arrVertex = new float[chunkSize * chunkSize * chunkSize * 24 * 8];// chunkSize^3�����飬ÿ�����鹲6*4=24�����㣬ÿ��������8����Ϣ
	this->numVertex = 0;
	this->arrFace = QVector<face>(chunkSize * chunkSize * chunkSize * 6);// ÿ������6����
	this->numFace = 0;
	this->heightMap = QVector<QVector<int>>(chunkSize, QVector<int>(chunkSize, 0));
	initHeightMap();
	this->typeDirTable = QVector<QVector<int>>(cubeList.length(), QVector<int>(3, 0));
	vbo.create();
}

Chunk::~Chunk() {
	vbo.destroy();
	delete[] arrVertex;
}

bool Chunk::isCubeExist(int x, int y, int z) {
	// chunk����Ϊû�з���
	if (x < 0 || y < 0 || z < 0 || x >= chunkSize || y >= chunkSize || z >= chunkSize) return false;
	if (map[x][y][z] == -1) {
		return false;
	}
	else return true;
}

void Chunk::buildChunk()
{
	/*
	*��������chunk
	*x, y, z: ����(0,0,0)���������������
	*/

	// ����
	float x = pos.x();
	float y = pos.y();
	float z = pos.z();
	program1->bind();
	int cubeNum = cubeList.length();
	int count = 0;// ��¼�ѻ��Ƶĵ�ĸ���
	for (int i = cubeNum - 1; i >= 0; --i) {// type
		for (int j = 0; j < 3; ++j) {// direction
			cubeList.at(i)->bindTexture(j);
			QMatrix4x4 modelMat;
			modelMat.translate(x, y, z);

			program1->setUniformValue(matrixUniform, projectMat * viewMat * modelMat);
			glDrawArrays(GL_QUADS, count, typeDirTable[i][j] * 4);// ÿ����4����
			count += typeDirTable[i][j] * 4;
			cubeList.at(i)->releaseTexture(j);
		}
	}
	program1->release();
}

void Chunk::setMap() {
	//stone*5, dirt*5, grass*1
#pragma omp parallel for collapse(3)
	for (int i = 0; i < chunkSize; ++i) {
		for (int k = 0; k < chunkSize; ++k) {
			for (int j = 0; j < chunkSize; ++j) {
				if (j <= heightMap[i][k] / 3) {
					map[i][j][k] = STONE;
				}
				else if (j <= heightMap[i][k] - 1) {
					map[i][j][k] = DIRT;
				}
				else if (j <= heightMap[i][k]) {
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
void Chunk::setPos(const float& x, const float& y, const float& z) {
	this->pos.setX(x);
	this->pos.setY(y);
	this->pos.setZ(z);

}

QVector3D Chunk::getPos() {
	return pos;
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
					if (isCubeExist(i - 1, j, k)) {// ����ߵ�������ڣ���ô��λ���������治���ӣ�ͬʱ��ߵ��������治����
						visible_info[i][j][k][LEFT] = false;
						visible_info[i - 1][j][k][RIGHT] = false;
					}
					if (isCubeExist(i, j - 1, k)) {// ����
						visible_info[i][j][k][BOTTOM] = false;
						visible_info[i][j - 1][k][TOP] = false;
					}
					if (isCubeExist(i, j, k - 1)) {// ����
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

bool cmp(const face &a, const face &b) {
	if (a.type == b.type) return a.dir < b.dir;
	return a.type > b.type;
}

void Chunk::generateVertexArray() {
	// �����ڵ���Ϣ�����ɶ�����Ϣ(λ�á���ɫ������)����һ��˳�����arrVertex
	// ���ڳ�ʼ���Լ�chunk�з�������ʱ����
	// ��¼������Ϣ��һ������4����
	resetVisible();
	calculateVisible();
	vertexInfo v[4];
	float cubeSize = cubeList[0]->getCubeSize();
	clearVertexes();
	clearFaces();
	// ����visible_info��ȡ�ڵ���Ϣ
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (map[i][j][k] == -1) continue;
				float offset_x = 2 * cubeSize * i;
				float offset_y = 2 * cubeSize * j;
				float offset_z = 2 * cubeSize * k;
				if (visible_info[i][j][k][FRONT]) {
					v[0] = vertexInfo{ -cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0};
					v[1] = vertexInfo{ cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0 };
					v[2] = vertexInfo{ cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0 };
					v[3] = vertexInfo{ -cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0 };
					// ����ƫ����
					for (int p = 0; p < 4; ++p) {
						v[p].pos_x += offset_x;
						v[p].pos_y += offset_y;
						v[p].pos_z += offset_z;
					}
					// �Ѹ������arrFace
					addFace(v, map[i][j][k], 1);
					numVertex += 4;
				}
				if (visible_info[i][j][k][BACK]) {
					v[0] = vertexInfo{ -cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0 };
					v[1] = vertexInfo{ -cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0 };
					v[2] = vertexInfo{ cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0 };
					v[3] = vertexInfo{ cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0 };
					// ����ƫ����
					for (int p = 0; p < 4; ++p) {
						v[p].pos_x += offset_x;
						v[p].pos_y += offset_y;
						v[p].pos_z += offset_z;
					}
					// �Ѹ������arrFace
					addFace(v, map[i][j][k], 1);
					numVertex += 4;
				}
				if (visible_info[i][j][k][LEFT]) {
					v[0] = vertexInfo{ -cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0 };
					v[1] = vertexInfo{ -cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0 };
					v[2] = vertexInfo{ -cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0 };
					v[3] = vertexInfo{ -cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0 };
					// ����ƫ����
					for (int p = 0; p < 4; ++p) {
						v[p].pos_x += offset_x;
						v[p].pos_y += offset_y;
						v[p].pos_z += offset_z;
					}
					// �Ѹ������arrFace
					addFace(v, map[i][j][k], 1);
					numVertex += 4;
				}
				if (visible_info[i][j][k][RIGHT]) {
					v[0] = vertexInfo{ cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0 };
					v[1] = vertexInfo{ cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0 };
					v[2] = vertexInfo{ cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0 };
					v[3] = vertexInfo{ cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0 };
					// ����ƫ����
					for (int p = 0; p < 4; ++p) {
						v[p].pos_x += offset_x;
						v[p].pos_y += offset_y;
						v[p].pos_z += offset_z;
					}
					// �Ѹ������arrFace
					addFace(v, map[i][j][k], 1);
					numVertex += 4;
				}
				if (visible_info[i][j][k][TOP]) {
					v[0] = vertexInfo{ -cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0 };
					v[1] = vertexInfo{ -cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0 };
					v[2] = vertexInfo{ cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0 };
					v[3] = vertexInfo{ cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0 };
					// ����ƫ����
					for (int p = 0; p < 4; ++p) {
						v[p].pos_x += offset_x;
						v[p].pos_y += offset_y;
						v[p].pos_z += offset_z;
					}
					// ���òݿ���ɫ
					for (int p = 0; p < 4 && map[i][j][k]==GRASS; ++p) {
						v[p].clr_r = 0.6f;
						v[p].clr_g = 1.0f;
						v[p].clr_b = 0.35f;
					}
					// �Ѹ������arrFace
					addFace(v, map[i][j][k], 0);
					numVertex += 4;
				}
				if (visible_info[i][j][k][BOTTOM]) {
					v[0] = vertexInfo{ -cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0 };
					v[1] = vertexInfo{ -cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0 };
					v[2] = vertexInfo{ cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0 };
					v[3] = vertexInfo{ cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0 };
					// ����ƫ����
					for (int p = 0; p < 4; ++p) {
						v[p].pos_x += offset_x;
						v[p].pos_y += offset_y;
						v[p].pos_z += offset_z;
					}
					// �Ѹ������arrFace
					addFace(v, map[i][j][k], 2);
					numVertex += 4;
				}
			}
		}
	}
	sort(arrFace.begin(), arrFace.end(), cmp);// ����type�Ӵ�С����,type��ͬʱ��dir��������
	// �ϲ���arrVertex
	int count = 0; // ��arrVertex����ӵĵ����
	for (int i = 0; i < numFace; ++i) {
		for (int j = 0; j < 4; ++j) {// ÿ������4����
			arrVertex[count++] = arrFace[i].v[j].pos_x;
			arrVertex[count++] = arrFace[i].v[j].pos_y;
			arrVertex[count++] = arrFace[i].v[j].pos_z;
			arrVertex[count++] = arrFace[i].v[j].clr_r;
			arrVertex[count++] = arrFace[i].v[j].clr_g;
			arrVertex[count++] = arrFace[i].v[j].clr_b;
			arrVertex[count++] = arrFace[i].v[j].tex_x;
			arrVertex[count++] = arrFace[i].v[j].tex_y;
		}
	}
}

void Chunk::setChunk(QMatrix4x4 projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* program1, bool isInit) {
	// ��������ɫ����ͶӰ�����
	this->program1 = program1;
	this->projectMat = projectMat;
	this->viewMat = viewMat;
	this->program1->bind();
	matrixUniform = this->program1->uniformLocation("mvpMat");
	this->program1->release();

	// �󶨲�������δ�buffer�ж�ȡ����
	vbo.bind();

	if (isInit)
		vbo.allocate(arrVertex, sizeof(float) * numVertex * 8);// numVertex�����㣬ÿ������8����Ϣ

	int attr = -1;
	attr = program1->attributeLocation("attrPos");
	program1->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	program1->enableAttributeArray(attr);

	attr = program1->attributeLocation("attrTexCoord");
	program1->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	program1->enableAttributeArray(attr);

	attr = program1->attributeLocation("attrTexColor");
	program1->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 8);
	program1->enableAttributeArray(attr);

	vbo.release();
}
// dir:[0,2]�ֱ��ʾtop, side, down
void Chunk::addFace(vertexInfo v[], int type, int dir) {
	face f;
	f.type = type;
	f.dir = dir;
	++typeDirTable[type][dir];
	for (int i = 0; i < 4; ++i) {
		f.v[i] = v[i];
	}
	arrFace[numFace++] = f;
}

void Chunk::initHeightMap() {
	// �����Ľǵ������ʼֵ
	static unsigned int seed = 0;
	srand(seed++);
	int low = chunkSize / 3;
	int high = chunkSize - 1;
	heightMap[0][0] = rand() % (high - low) + low;
	heightMap[chunkSize - 1][0] = rand() % (high - low) + low;
	heightMap[0][chunkSize - 1] = rand() % (high - low) + low;
	heightMap[chunkSize - 1][chunkSize - 1] = rand() % (high - low) + low;
#pragma omp parallel for collapse(2)
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			heightMap[i][j] = valueNoise(i, j);
		}
	}
}

int Chunk::valueNoise(int& i, int& j) const{
	if ((i == 0 && j == 0) || 
		(i == 0 && j == chunkSize - 1) || 
		(i == chunkSize - 1 && j == 0) || 
		(i == chunkSize - 1 && j == chunkSize - 1)) {
		return heightMap[i][j];
	}
	else {
		// ��һ��
		float i_norm = (float)i / (chunkSize - 1);
		float j_norm = (float)j / (chunkSize - 1);
		float a = heightMap[0][0];
		float b = heightMap[chunkSize - 1][0];
		float c = heightMap[0][chunkSize - 1];
		float d = heightMap[chunkSize - 1][chunkSize - 1];
		// �û������߼������Բ�ֵ���Ա���t
		float t_i = easeCurve(i_norm);
		float t_j = easeCurve(j_norm);
		// �������Բ�ֵ
		float noise_i1 = lern(t_i, a, b);
		float noise_i2 = lern(t_i, c, d);
		return (int)lern(t_j, noise_i1, noise_i2);// �Ŵ�ԭʼ�߶�
	}
}

float Chunk::easeCurve(float& t) const {
	return t * t * (3 - 2 * t);
}

float Chunk::lern(float& t, float& n1, float& n2) const {
	return n1 + (n2 - n1) * t;
}

void Chunk::setCollision() {
	/*
	* ����pos��map������ײ���
	*/
	float x = pos.x();
	float y = pos.y();
	float z = pos.z();
	float cubeSize = cubeList[0]->getCubeSize();
	float spacing = 2 * cubeSize;
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (map[i][j][k] != -1) {// ���ڷ���ʱ
					float pos_x = x + i * spacing;
					float pos_y = y + j * spacing;
					float pos_z = z + k * spacing;
					QVector3D vector1 = QVector3D(pos_x - cubeSize, pos_y - cubeSize, pos_z - cubeSize);
					QVector3D vector2 = QVector3D(pos_x + cubeSize, pos_y + cubeSize, pos_z + cubeSize);
					objects.push_back(object(vector1, vector2));
				}
			}
		}
	}

}

void Chunk::addCube(QVector3D relative_pos, QMatrix4x4 m_projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* m_program1, int type) {
	map[relative_pos.x()][relative_pos.y()][relative_pos.z()] = type;
	generateVertexArray();
	setChunk(m_projectMat, viewMat, m_program1, true);
}

void Chunk::deleteCube(QVector3D relative_pos, QMatrix4x4 m_projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram*  m_program1) {
	map[relative_pos.x()][relative_pos.y()][relative_pos.z()] = -1;
	generateVertexArray();
	setChunk(m_projectMat, viewMat, m_program1, true);
}

void Chunk::clearVertexes() {
	for (int i = 0; i < numVertex; ++i)
		arrVertex[i] = 0.0;
	numVertex = 0;
}
void Chunk::clearFaces() {
	for (int i = 0; i < numFace; ++i) {
		arrFace[i] = face();
	}
	for (int i = 0; i < cubeList.length(); ++i) {
		for (int j = 0; j < 3; ++j) {
			typeDirTable[i][j] = 0;
		}
	}
	numFace = 0;
}

void Chunk::resetVisible() {
#pragma omp parallel for collapse(4)
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				for (int d = 0; d < 6; ++d){
					visible_info[i][j][k][d] = true;
				}
			}
		}
	}
}
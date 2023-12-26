#include "Cube.h"

Cube::Cube(float size, QStringList& Images, int type)
{
	/*
	初始化方块的大小和贴图
	size：边长的1/2
	Images：贴图，top、side、bottom
	*/

	resetVisible();
	cubeSize = size;
	cubeType = type;
	
	for (int i = 0; i < 3; ++i) {
		texture[i] = new QOpenGLTexture(QImage(Images[i]).mirrored(), QOpenGLTexture::GenerateMipMaps);
		texture[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
		texture[i]->setMagnificationFilter(QOpenGLTexture::Nearest);
		texture[i]->setWrapMode(QOpenGLTexture::Repeat);
	}

	vbo.create();
}

Cube::~Cube() {
	vbo.destroy();
}

void Cube::setCube(QMatrix4x4 projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* program1)
{
	this->program1 = program1;
	this->projectMat = projectMat;
	this->viewMat = viewMat;
	this->program1->bind();
	matrixUniform = this->program1->uniformLocation("mvpMat");
	this->program1->release();
}

void Cube::drawCube(float x, float y, float z) 
{	
	program1->bind();
	texture[0]->bind();
	QMatrix4x4 modelMat;
	modelMat.translate(x, y, z);

	program1->setUniformValue(matrixUniform, projectMat * viewMat * modelMat);
	glDrawArrays(GL_QUADS, 0, 24);

	texture[0]->release();
	program1->release();
	
	QVector3D vector1 = QVector3D(x - cubeSize, y - cubeSize, z - cubeSize);
	QVector3D vector2 = QVector3D(x + cubeSize, y + cubeSize, z + cubeSize);
	objects.push_back(object(vector1, vector2));
}

void Cube::makeOneCube(bool isInit)
{
	float arrVertex[]{
		// 位置							// 颜色			  // 纹理
		-cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0,
		-cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0,
		cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f,	1.0, 1.0,
		cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0,
		
		-cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0,
		cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0,
		cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f,	1.0, 1.0,
		-cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0,

		-cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0,
		-cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0,
		cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0,
		cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0,
		
		cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0,
		cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0,
		cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0,
		cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0,
		
		-cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0,
		-cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0,
		-cubeSize, cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0,
		-cubeSize, cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0,
		
		-cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 0.0,
		-cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 0.0, 1.0,
		cubeSize, -cubeSize, -cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 1.0,
		cubeSize, -cubeSize, cubeSize, 1.0f, 1.0f, 1.0f, 1.0, 0.0
	};



	vbo.bind();
	if(isInit)
		vbo.allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = program1->attributeLocation("attrPos");
	program1->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	program1->enableAttributeArray(attr);

	attr = program1->attributeLocation("attrTexCoord");
	program1->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	program1->enableAttributeArray(attr);

	vbo.release();
}

void Cube::setVisible(vector<bool> &visible_info) {
	for (int i = 0; i < 6; ++i) {
		isVisible[i] = visible_info[i];
	}
}

void Cube::resetVisible() {
	for (int i = 0; i < 6; ++i) {
		isVisible[i] = true;
	}
}

float Cube::getCubeSize() {
	return cubeSize;
}

void Cube::bindTexture(int dir) const{
	texture[dir]->bind();
}

void Cube::releaseTexture(int dir) const{
	texture[dir]->release();
}
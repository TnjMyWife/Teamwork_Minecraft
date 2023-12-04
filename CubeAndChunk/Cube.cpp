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
	
	texture = new QOpenGLTexture(QImage("texture/grass_block_side.png").mirrored(), QOpenGLTexture::GenerateMipMaps);
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Nearest);
	texture->setWrapMode(QOpenGLTexture::Repeat);

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
	texture->bind();
	QMatrix4x4 modelMat;
	modelMat.translate(x, y, z);

	program1->setUniformValue(matrixUniform, projectMat * viewMat * modelMat);
	glDrawArrays(GL_QUADS, 0, 24);

	texture->release();
	program1->release();

}

void Cube::makeOneCube()
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

void Cube::setInvisible(int i) {
	isVisible[i] = false;
}

void Cube::resetVisible() {
	for (int i = 0; i < 6; ++i) {
		isVisible[i] = true;
	}
}

float Cube::getCubeSize() {
	return cubeSize;
}
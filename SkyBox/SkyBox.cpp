#include "SkyBox.h"

SkyBox::SkyBox()
{
	for (int i = 0; i < 6; ++i) {
		texture[i] = nullptr;
	}
}

SkyBox::~SkyBox()
{
	for (int i = 0; i < 6; ++i) {
		vbo[i].destroy();
	}

}

void SkyBox::initSkyBox(QOpenGLShaderProgram* m_program2)
{
	m_program2->bind();
	m_matrixUniform = m_program2->uniformLocation("mvpMat");
	m_program2->release();
	for (int i = 0; i < 6; ++i) {
		vbo[i].create();
	}
	loadGLTextures();
}



void SkyBox::loadGLTextures()
{
	QStringList Images;
	Images << "texture/top.png";
	Images << "texture/bottom.png";
	Images << "texture/left.png";
	Images << "texture/right.png";
	Images << "texture/front.png";
	Images << "texture/back.png";

	for (int i = 0; i < 6; i++) {
		texture[i] = new QOpenGLTexture(QImage(Images.at(i)).mirrored(), QOpenGLTexture::GenerateMipMaps);
		texture[i]->setMinificationFilter(QOpenGLTexture::Linear);
		texture[i]->setMagnificationFilter(QOpenGLTexture::Linear);
		texture[i]->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
		texture[i]->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
	}


}

void SkyBox::makeTop(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// 顶面
		-skySize, skySize, -skySize, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		skySize, skySize, -skySize, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		skySize, skySize, skySize, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		-skySize, skySize, skySize, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
	};

	vbo[0].bind();
	vbo[0].allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = m_program2->attributeLocation("attrPos");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexCoord");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexColor");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	vbo[0].release();
}

void SkyBox::makeBottom(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// 底面
		-skySize, -skySize, -skySize, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-skySize, -skySize, skySize, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		skySize, -skySize, skySize, 1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
		skySize, -skySize, -skySize, 1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		-skySize, skySize, -skySize, 1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	};

	vbo[1].bind();
	vbo[1].allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = m_program2->attributeLocation("attrPos");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexCoord");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexColor");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	vbo[1].release();
	
}

void SkyBox::makeLeft(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		
		// 左面
		-skySize, -skySize, -skySize, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-skySize, skySize, -skySize, 1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		-skySize, skySize, skySize, 1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
		-skySize, -skySize, skySize, 1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	};

	vbo[2].bind();
	vbo[2].allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = m_program2->attributeLocation("attrPos");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexCoord");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexColor");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	vbo[2].release();
}

void SkyBox::makeRight(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// 右面
		skySize, -skySize, -skySize, 1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		skySize, -skySize, skySize, 1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
		skySize, skySize, skySize, 1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		skySize, skySize, -skySize, 1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	};

	vbo[3].bind();
	vbo[3].allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = m_program2->attributeLocation("attrPos");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexCoord");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexColor");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	vbo[3].release();
}

void SkyBox::makeFront(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// 位置							// 颜色			  // 纹理
		// 后面
		-skySize, -skySize, -skySize, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		skySize, -skySize, -skySize, 1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
		skySize, skySize, -skySize, 1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		-skySize, skySize, -skySize, 1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
		
	};

	vbo[4].bind();
	vbo[4].allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = m_program2->attributeLocation("attrPos");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexCoord");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexColor");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	vbo[4].release();
}

void SkyBox::makeBack(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// 位置							// 颜色			  // 纹理

		// 前面
		-skySize, -skySize, skySize, 1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
		-skySize, skySize, skySize, 1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		skySize, skySize, skySize, 1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
		skySize, -skySize, skySize, 1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	};

	vbo[5].bind();
	vbo[5].allocate(arrVertex, sizeof(arrVertex));

	int attr = -1;
	attr = m_program2->attributeLocation("attrPos");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexCoord");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	attr = m_program2->attributeLocation("attrTexColor");
	m_program2->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 8);
	m_program2->enableAttributeArray(attr);

	vbo[5].release();
}

void SkyBox::drawSky(QVector3D characterPos, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2)
{
	m_program2->bind();
	
	QMatrix4x4 movBody;
	movBody.translate(characterPos.x(), characterPos.y(), characterPos.z());		// 设置人物的位置，这个位置随着移动而变化（同相机）

	texture[0]->bind();
	makeTop(m_program2);
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * movBody);
	glDrawArrays(GL_QUADS, 0, 4);
	texture[0]->release();

	texture[1]->bind();
	makeBottom(m_program2);
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * movBody);
	glDrawArrays(GL_QUADS, 0, 4);
	texture[1]->release();

	texture[2]->bind();
	makeLeft(m_program2);
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * movBody);
	glDrawArrays(GL_QUADS, 0, 4);
	texture[2]->release();

	texture[3]->bind();
	makeRight(m_program2);
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * movBody);
	glDrawArrays(GL_QUADS, 0, 4);
	texture[3]->release();

	texture[4]->bind();
	makeFront(m_program2);
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * movBody);
	glDrawArrays(GL_QUADS, 0, 4);
	texture[4]->release();

	texture[5]->bind();
	makeBack(m_program2);
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * movBody);
	glDrawArrays(GL_QUADS, 0, 4);
	texture[5]->release();



	m_program2->release();
}
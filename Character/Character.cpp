#include "Character.h"

Character::Character():
	angle(0.0f),
	swingSpeed(10.0f),
	texture(nullptr)
{

}

Character::~Character()
{
	for (int i = 0; i < 3; i++)
		vbo[i].destroy();
}

void Character::initCharacter(QOpenGLShaderProgram* m_program2)
{
	m_program2->bind();
	m_matrixUniform = m_program2->uniformLocation("mvpMat");
	m_program2->release();
	for (int i = 0; i < 3; i++)
		vbo[i].create();
	loadGLTextures();
}

void Character::setAngle(float angle)
{
	this->angle = angle;
}

void Character::swing()
{
	angle += swingSpeed;
	if (angle >= 45.0f || angle <= -45.0f) {
		swingSpeed = -swingSpeed; // ��ת��ת����
	}
}

void Character::drawHead(QVector3D cameraFront, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2)
{
	QMatrix4x4 modelMat;
	modelMat.translate(0.0f, 0.28f, 0.0f);   // ����ͷ���ĳ�ʼλ��


	// ͷ�������ת�� 
	QVector3D rotationAxis = QVector3D::crossProduct(QVector3D(0.0f, 0.0f, 1.0f), cameraFront);			// ��Ϊ��ʼ����Ϊ(0,0,1), �������������z���ˣ��õ���ת�� */
	qreal rotationAngle = qRadiansToDegrees(qAcos(QVector3D::dotProduct(QVector3D(0.0f, 0.0f, 1.0f), cameraFront)));	// ������ת�Ƕ� 
	QMatrix4x4 rotMat;
	rotMat.rotate(rotationAngle, rotationAxis.x(), rotationAxis.y(), rotationAxis.z());		// ��ת 

	modelMat = modelMat * rotMat;

	
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * modelMat);

	glDrawArrays(GL_QUADS, 0, 24);

}

void Character::drawBody(QVector3D cameraFront, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2)
{
	QMatrix4x4 modelMat;
	modelMat.translate(0.0f, 0.18f, 0.0f);  /* ���������λ�� */

	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * modelMat);

	glDrawArrays(GL_QUADS, 0, 24);

}

void Character::drawRightArm(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2) {
	QMatrix4x4 tranMat1, rotMat1, tranMat2;
	tranMat1.translate(-0.06f, 0.22f, 0.0f);  // �����ֱ۳�ʼλ��
	rotMat1.rotate(angle, 1.0f, 0.0f, 0.0f);	// ��x����ת��angle���ƶ��¼������ʵʱ����
	tranMat2.translate(0.0f, -0.04f, 0.0f);  // ������0.04
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * tranMat1 * rotMat1 * tranMat2);
	glDrawArrays(GL_QUADS, 0, 24);


}

void Character::drawLeftArm(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2) {
	QMatrix4x4 tranMat1, rotMat1, tranMat2;
	tranMat1.translate(0.06f, 0.22f, 0.0f);  // �����ֱ۳�ʼλ��
	rotMat1.rotate(-angle, 1.0f, 0.0f, 0.0f);	// ��x����ת��angle���ƶ��¼������ʵʱ����
	tranMat2.translate(0.0f, -0.04f, 0.0f);  // ������0.04
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * tranMat1 * rotMat1 * tranMat2);
	glDrawArrays(GL_QUADS, 0, 24);
}

void Character::drawLeftLeg(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2) {
	QMatrix4x4 tranMat1, rotMat1, tranMat2;

	tranMat1.translate(0.02f, 0.12f, 0.0f);  // �����ȳ�ʼλ��
	rotMat1.rotate(angle, 1.0f, 0.0f, 0.0f);	// ��x����ת��angle���ƶ��¼������ʵʱ����
	tranMat2.translate(0.0f, -0.06f, 0.0f);  // ������0.06
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * tranMat1 * rotMat1 * tranMat2);
	glDrawArrays(GL_QUADS, 0, 24);
}

void Character::drawRightLeg(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2) {
	QMatrix4x4 tranMat1, rotMat1, tranMat2;

	tranMat1.translate(-0.02f, 0.12f, 0.0f);  // �����ȳ�ʼλ��
	rotMat1.rotate(-angle, 1.0f, 0.0f, 0.0f);	// ��x����ת��angle���ƶ��¼������ʵʱ����
	tranMat2.translate(0.0f, -0.06f, 0.0f);  // ������0.06
	m_program2->setUniformValue(m_matrixUniform, proAndViewMat * tranMat1 * rotMat1 * tranMat2);
	glDrawArrays(GL_QUADS, 0, 24);
}

void Character::drawCharacter(QVector3D cameraFront, QVector3D characterPos, 
							  QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2)
{
	m_program2->bind();
	texture->bind();
	QMatrix4x4 movBody;
	movBody.translate(characterPos.x(), characterPos.y(), characterPos.z());		// ���������λ�ã����λ�������ƶ����仯��ͬ�����

	makeHead(m_program2);
	drawHead(cameraFront, proAndViewMat * movBody, m_program2);

	// ���������ӽ�ת��
	QVector3D xoyCameraFront = QVector3D(cameraFront.x(), 0.0f, cameraFront.z());		// �����������ͶӰ��xozƽ��
	QVector3D rotationAxis = QVector3D::crossProduct(QVector3D(0.0f, 0.0f, 1.0f), xoyCameraFront);		// ��z���˵õ���ת�ᣨ�˴�ʵ���ϵõ�Ϊy�ᣩ
	qreal rotationAngle = qRadiansToDegrees(qAcos(QVector3D::dotProduct(QVector3D(0.0f, 0.0f, 1.0f), xoyCameraFront)));		// ��ת�Ƕȼ���
	
	QMatrix4x4 rotMat;
	rotMat.rotate(rotationAngle, rotationAxis.x(), rotationAxis.y(), rotationAxis.z());


	makeBody(m_program2);
	drawBody(cameraFront, proAndViewMat * movBody * rotMat, m_program2);

	makeOne(m_program2);
	drawLeftArm(proAndViewMat * movBody * rotMat, m_program2);

	makeOne(m_program2);
	drawRightArm(proAndViewMat * movBody * rotMat, m_program2);

	makeOne(m_program2);
	drawLeftLeg(proAndViewMat * movBody * rotMat, m_program2);

	makeOne(m_program2);
	drawRightLeg(proAndViewMat * movBody * rotMat, m_program2);

	texture->release();
	m_program2->release();
}


void Character::loadGLTextures()
{
	texture = new QOpenGLTexture(QImage("texture/zombie.png").mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Nearest);
	texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Character::makeHead(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// λ��							// ��ɫ			  // ����
		// ǰ��
		-0.04f, -0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 16.0f / 32.0f,
		0.04f, -0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 16.0f / 32.0f,
		0.04f, 0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 24.0f / 32.0f,
		-0.04f, 0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 24.0f / 32.0f,

		// ����
		-0.04f, -0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 24.0f / 56.0f, 16.0f / 32.0f,
		-0.04f, 0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 24.0f / 56.0f, 24.0f / 32.0f,
		0.04f, 0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 31.0f / 56.0f, 24.0f / 32.0f,
		0.04f, -0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 31.0f / 56.0f, 16.0f / 32.0f,

		// ����
		-0.04f, 0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 0.0f, 16.0f / 32.0f,
		-0.04f, 0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 16.0f / 32.0f,
		0.04f, 0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 24.0f / 32.0f,
		0.04f, 0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 0.0f, 24.0f / 32.0f,

		// ����
		-0.04f, -0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 17.0f / 56.0f, 32.0f / 32.0f,
		0.04f, -0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 23.0f / 56.0f, 32.0f / 32.0f,
		0.04f, -0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 23.0f / 56.0f, 24.0f / 32.0f,
		-0.04f, -0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 17.0f / 56.0f, 24.0f / 32.0f,

		// ����
		0.04f, -0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 24.0f / 56.0f, 16.0f / 32.0f,
		0.04f, 0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 24.0f / 56.0f, 24.0f / 32.0f,
		0.04f, 0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 24.0f / 32.0f,
		0.04f, -0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 16.0f / 32.0f,

		// ����
		-0.04f, -0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 0.0f, 16.0f / 32.0f,
		-0.04f, -0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 16.0f / 32.0f,
		-0.04f, 0.04f, 0.04f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 24.0f / 32.0f,
		-0.04f, 0.04f, -0.04f, 1.0f, 1.0f, 1.0f, 0.0f, 24.0f / 32.0f
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

	vbo[0].release();
}

void Character::makeBody(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// λ��							// ��ɫ			  // ����
		// ǰ��
		-0.04f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 20.0f / 56.0f, 0.0f / 32.0f,
		0.04f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 28.0f / 56.0f, 0.0f / 32.0f,
		0.04f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 28.0f / 56.0f, 12.0f / 32.0f,
		-0.04f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 20.0f / 56.0f, 12.0f / 32.0f,

		// ����
		-0.04f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 40.0f / 56.0f, 0.0f / 32.0f,
		-0.04f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 40.0f / 56.0f, 12.0f / 32.0f,
		0.04f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 32.0f / 56.0f, 12.0f / 32.0f,
		0.04f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 32.0f / 56.0f, 0.0f / 32.0f,

		// ����
		-0.04f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 20.1f / 56.0f, 16.0f / 32.0f,
		-0.04f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 20.1f / 56.0f, 12.0f / 32.0f,
		0.04f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 28.0f / 56.0f, 12.0f / 32.0f,
		0.04f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 28.0f / 56.0f, 16.0f / 32.0f,

		// ����
		-0.04f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 28.0f / 56.0f, 16.0f / 32.0f,
		0.04f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 35.5f / 56.0f, 16.0f / 32.0f,
		0.04f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 35.5f / 56.0f, 12.0f / 32.0f,
		-0.04f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 28.0f / 56.0f, 12.0f / 32.0f,

		// ����
		0.04f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 20.0f / 56.0f, 0.0f,
		0.04f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 20.0f / 56.0f, 12.0f / 32.0f,
		0.04f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 12.0f / 32.0f,
		0.04f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 0.0,

		// ����
		-0.04f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 0.0,
		-0.04f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 20.0f / 56.0f, 0.0,
		-0.04f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 20.0f / 56.0f, 12.0f / 32.0f,
		-0.04f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 16.0f / 56.0f, 12.0f / 32.0f,
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

	vbo[1].release();
}

void Character::makeOne(QOpenGLShaderProgram* m_program2)
{
	float arrVertex[]{
		// λ��							// ��ɫ			  // ����
		// ǰ��
		-0.02f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 4.0f / 56.0f, 0.0f / 32.0f,
		0.02f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 0.0f / 32.0f,
		0.02f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 8.0f / 56.0f, 12.0f / 32.0f,
		-0.02f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 4.0f / 56.0f, 12.0f / 32.0f,

		// ����
		-0.02f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 15.5f / 56.0f, 0.0f / 32.0f,
		-0.02f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 15.5f / 56.0f, 12.0f / 32.0f,
		0.02f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 12.0f / 56.0f, 12.0f / 32.0f,
		0.02f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 12.0f / 56.0f, 0.0f / 32.0f,

		// ����
		-0.02f, 0.06f, -0.02f, 0.6f, 1.0f, 0.35f, 0.0f, 0.0f,
		-0.02f, 0.06f, 0.02f, 0.6f, 1.0f, 0.35f, 0.0f, 12.0f / 32.0f,
		0.02f, 0.06f, 0.02f, 0.6f, 1.0f, 0.35f, 0.02f, 12.0f / 32.0f,
		0.02f, 0.06f, -0.02f, 0.6f, 1.0f, 0.35f, 0.02f, 0.0f,

		// ����
		-0.02f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.02f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 0.02f, 0.0f,
		0.02f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 0.02f, 12.0f / 32.0f,
		-0.02f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 0.0f, 12.0f / 32.0f,

		// ����
		0.02f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 11.5f / 56.0f, 0.0f / 32.0f,
		0.02f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 11.5f / 56.0f, 12.0f / 32.0f,
		0.02f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 8.5f / 56.0f, 12.0f / 32.0f,
		0.02f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 8.5f / 56.0f, 0.0f / 32.0f,

		// ����
		-0.02f, -0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 0.0f / 56.0f, 0.0f / 32.0f,
		-0.02f, -0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 3.5f / 56.0f, 0.0f / 32.0f,
		-0.02f, 0.06f, 0.02f, 1.0f, 1.0f, 1.0f, 3.5f / 56.0f, 12.0f / 32.0f,
		-0.02f, 0.06f, -0.02f, 1.0f, 1.0f, 1.0f, 0.5f / 56.0f, 12.0f / 32.0f,
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

	vbo[2].release();
}

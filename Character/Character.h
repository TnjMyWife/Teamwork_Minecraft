#pragma once
#include <QVector3D>
#include <GL/glew.h>
#include <QtMath>
#include <QOpenGLWidget>
#include <qgl.h> 
#include <QString>

class Character
{
public:
	Character();
	~Character();
	void setAngle(float angle);		// ����angle
	void swing();						// �ڶ�����
	void drawHead(QVector3D cameraFront);	// ��һ��ͷ
	void drawBody();				// ����
	void drawOne();					// ��֮֫һ
	void drawArm();
	void drawLeg();
	void drawCharacter(QVector3D cameraFront, QVector3D characterPos);		// �������Ҫ����������������
	void loadGLTextures();
private:
	float angle;
	float swingSpeed;
	GLuint texture[3];
};
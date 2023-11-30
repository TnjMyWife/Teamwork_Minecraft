#pragma once
#include <QVector3D>
#include <GL/glew.h>
#include <QtMath>
class Character
{
public:
	Character();
	~Character();
	void setAngle(float angle);
	void swing();
	void drawHead(QVector3D cameraFront, GLuint* texture);
	void drawBody();
	void drawOne();
	void drawArm();
	void drawLeg();
	void drawCharacter(QVector3D cameraFront, QVector3D characterPos, GLuint* texture);
	
private:
	float angle;
	float swingSpeed;
};
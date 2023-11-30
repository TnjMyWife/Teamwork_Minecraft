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
	void setAngle(float angle);		// 设置angle
	void swing();						// 摆动过程
	void drawHead(QVector3D cameraFront);	// 画一个头
	void drawBody();				// 身体
	void drawOne();					// 四肢之一
	void drawArm();
	void drawLeg();
	void drawCharacter(QVector3D cameraFront, QVector3D characterPos);		// 画人物，需要相机方向和任务坐标
	void loadGLTextures();
private:
	float angle;
	float swingSpeed;
	GLuint texture[3];
};
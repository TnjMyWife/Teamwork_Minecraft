#pragma once
#include <QVector3D>
#include <GL/glew.h>
#include <QtMath>
#include <QOpenGLWidget>
#include <qgl.h> 
#include <QString>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>

class Character
{
public:
	Character();
	~Character();
	void initCharacter(QOpenGLShaderProgram* m_program2);
	void setAngle(float angle);		// 设置angle
	void swing();						// 摆动过程
	void makeHead(QOpenGLShaderProgram* m_program2);			// 绑定头vbo
	void makeBody(QOpenGLShaderProgram* m_program2);			// 绑定身体vbo
	void makeOne(QOpenGLShaderProgram* m_program2);			// 绑定四肢之一vbo

	void drawHead(QVector3D cameraFront, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);	// 画一个头
	void drawBody(QVector3D cameraFront, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);	// 身体		
	void drawLeftArm(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawRightArm(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawLeftLeg(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawRightLeg(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawCharacter(QVector3D cameraFront, QVector3D characterPos,
						QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
					// 画人物，需要相机方向和人物坐标
	void loadGLTextures();			// 纹理加载

private:
	float angle;
	float swingSpeed;
	QOpenGLBuffer vbo[3];
	QOpenGLTexture* texture;
	int m_matrixUniform;

};
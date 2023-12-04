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
	void setAngle(float angle);		// ����angle
	void swing();						// �ڶ�����
	void makeHead(QOpenGLShaderProgram* m_program2);			// ��ͷvbo
	void makeBody(QOpenGLShaderProgram* m_program2);			// ������vbo
	void makeOne(QOpenGLShaderProgram* m_program2);			// ����֮֫һvbo

	void drawHead(QVector3D cameraFront, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);	// ��һ��ͷ
	void drawBody(QVector3D cameraFront, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);	// ����		
	void drawLeftArm(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawRightArm(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawLeftLeg(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawRightLeg(QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
	void drawCharacter(QVector3D cameraFront, QVector3D characterPos,
						QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);
					// �������Ҫ����������������
	void loadGLTextures();			// �������

private:
	float angle;
	float swingSpeed;
	QOpenGLBuffer vbo[3];
	QOpenGLTexture* texture;
	int m_matrixUniform;

};
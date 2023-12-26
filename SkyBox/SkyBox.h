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


class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void initSkyBox(QOpenGLShaderProgram* m_program2);
	void makeTop(QOpenGLShaderProgram* m_program2);			// 绑定头vbo
	void makeBottom(QOpenGLShaderProgram* m_program2);
	void makeLeft(QOpenGLShaderProgram* m_program2);			// 绑定头vbo
	void makeRight(QOpenGLShaderProgram* m_program2);
	void makeFront(QOpenGLShaderProgram* m_program2);			// 绑定头vbo
	void makeBack(QOpenGLShaderProgram* m_program2);
	void drawSky(QVector3D characterPos, QMatrix4x4 proAndViewMat, QOpenGLShaderProgram* m_program2);	// 画人物，需要相机方向和人物坐标
	void loadGLTextures();			// 纹理加载

private:
	QOpenGLBuffer vbo[6];
	QOpenGLTexture* texture[6];
	int m_matrixUniform;
	float skySize = 25.0f;
};
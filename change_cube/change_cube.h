#pragma once
#include "../CubeAndChunk/Cube.h"
#include "../Camera/Camera.h"
#include "../CubeAndChunk/chunkList.h"

#define ADD false
#define DELETE true

class change_cube {
public:
	change_cube();
	~change_cube();
	QVector3D get_relative_position(QVector3D vector, float h, QVector3D base);
	void tochange_cube(QMatrix4x4 m_projectMat, QOpenGLShaderProgram* m_program1, Camera* camera, Collision& collision, bool action, int args);

};
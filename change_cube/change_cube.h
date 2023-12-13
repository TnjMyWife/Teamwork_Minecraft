#pragma once
#include "../CubeAndChunk/Cube.h"
#include "../Camera/Camera.h"

#define ADD false
#define DELETE true

class change_cube {	
public:
	change_cube();
	~change_cube();
	QVector3D get_relative_position(QVector3D vector, float h);
	void tochange_cube(Cube* cube, Camera* camera, Collision& collision, bool action, int args);

};


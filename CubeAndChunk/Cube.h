#pragma once
#include <GL/glew.h>
#include <QString>
#include <QOpenGLWidget>
#include <qgl.h>  
#include "../Collision/Collision.h"

enum blockType { GRASS, DIRT, STONE };
enum direction { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };

class Cube // �����࣬���з���ĳ��󣻱���������Ĵ�С���󶨵Ĳ��ʡ�Ӳ�ȵȣ����������Ʒ����
{
protected:
    float cubeSize;
    GLuint texture[3];//top, side, bottom
    bool isVisible[6];// ǰ����������
    int cubeType;

public:
    Cube(float size, QStringList& Images, int type);
    ~Cube();
    void drawCube(float x, float y, float z);
    void Cube::setInvisible(int i);
    void Cube::resetVisible();
    float getCubeSize();

};





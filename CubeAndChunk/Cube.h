#pragma once
#include <GL/glew.h>
#include <QString>
#include <QOpenGLWidget>
#include <qgl.h>  
#include "../Collision/Collision.h"
#include <vector>
using namespace std;

enum blockType { GRASS, DIRT, STONE };
enum direction { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };

class Cube // 方块类，所有方块的抽象；变量：方块的大小、绑定的材质、硬度等；方法：绘制方块等
{
protected:
    float cubeSize;
    GLuint texture[3];//top, side, bottom
    bool isVisible[6];// 前后左右上下
    int cubeType;

public:
    Cube(float size, QStringList& Images, int type);
    ~Cube();
    void drawCube(float x, float y, float z);
    void setVisible(vector<bool> &visible_info);
    void resetVisible();
    float getCubeSize();

};





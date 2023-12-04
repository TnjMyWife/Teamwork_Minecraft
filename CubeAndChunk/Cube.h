#include<iostream>
#include <GL/glew.h>
#include <QString>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <qgl.h>  
#include <qmatrix4x4.h>
// #include "../Collision/Collision.h"
#include "../Collision/Collision.h"
#include <vector>
using namespace std;


enum blockType { GRASS, DIRT, STONE };
enum direction { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };

class Cube // 方块类，所有方块的抽象；变量：方块的大小、绑定的材质、硬度等；方法：绘制方块等
{
protected:
    float cubeSize;
    QOpenGLTexture* texture;//top, side, bottom
    bool isVisible[6];// 前后左右上下
    int cubeType;
    int matrixUniform;
    QOpenGLBuffer vbo;
    QMatrix4x4 projectMat;
    QMatrix4x4 viewMat;
    QOpenGLShaderProgram* program1;

public:
    Cube(float size, QStringList& Images, int type);
    ~Cube();
    void drawCube(float x, float y, float z);
    void setVisible(vector<bool> &visible_info);
    void resetVisible();
    float getCubeSize();

    void setCube(QMatrix4x4 projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* program1);
    void makeOneCube();

};





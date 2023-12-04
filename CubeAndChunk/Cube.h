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

class Cube // �����࣬���з���ĳ��󣻱���������Ĵ�С���󶨵Ĳ��ʡ�Ӳ�ȵȣ����������Ʒ����
{
protected:
    float cubeSize;
    QOpenGLTexture* texture;//top, side, bottom
    bool isVisible[6];// ǰ����������
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





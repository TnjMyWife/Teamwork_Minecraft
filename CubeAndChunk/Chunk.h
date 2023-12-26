#pragma once
#include <QVector>
#include <QVector3D>
#include <omp.h>
#include <vector>
#include "cubeList.h"
using namespace std;

typedef struct v {
    float pos_x = 0;
    float pos_y = 0;
    float pos_z = 0;
    float clr_r = 0;
    float clr_g = 0;
    float clr_b = 0;
    float tex_x = 0;
    float tex_y = 0;
} vertexInfo;// 顶点信息（位置、颜色、纹理坐标）

typedef struct f {
    int type = -1;// 面属于哪个方块
    int dir = 0;// 面的方向
    vertexInfo v[4];
} face;

class Chunk
{
private:
    QVector<QVector<QVector<int>>> map;// 存储chunk内部每个位置的方块类型，三个维度分别对应x, y, z
    QVector3D pos;// 绘制起始位置
    int chunkSize;
    vector<vector<vector<vector<bool>>>> visible_info;// (chunkSize,chunkSize,chunkSize,6)的矩阵，记录每个方块的每个面是否可视, true代表该面可视
    float* arrVertex;// 绘制chunk的顶点数组
    int numVertex;// arrVertex中的顶点数
    int matrixUniform;
    QOpenGLBuffer vbo;
    QMatrix4x4 projectMat;
    QMatrix4x4 viewMat;
    QOpenGLShaderProgram* program1;
    QVector<face> arrFace;// 维护需要绘制的面，排序后合并至arrVertex
    int numFace;// arrFace中面的个数
    QVector<QVector<int>> typeDirTable;// 统计面的类型和朝向的表格
    QVector<QVector<int>> heightMap;// chunkSize*chunkSize, 记录高度信息

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// 检查chunk内(x, y, z)位置的方块是否存在，若存在，相邻位置的方块不用绘制表面
    void buildChunk();
    void setMap();
    void setPos(const float& x, const float& y, const float& z);// 设置chunk(0, 0, 0)位置方块的坐标
    QVector3D getPos();// 返回pos
    void setChunkSize(int size);
    void calculateVisible();// 计算chunk内部每个方块的每个面的可视情况
    int getChunkSize();
    void generateVertexArray();// 根据地图和遮挡情况生成顶点数组
    void setChunk(QMatrix4x4 projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* program1, bool isInit = false);// 绘制Chunk前的必要准备
    void addFace(vertexInfo v[], int type, int dir);// 添加一个待绘制的面，等待合并至arrVertex
    void initHeightMap();// 对高度信息进行随机初始化
    int valueNoise(int& i, int& j) const;// 根据位置计算噪声
    float easeCurve(float& t) const;
    float lern(float& t, float& n1, float& n2) const;// 计算线性插值
    void setCollision();// 依据pos、map添加碰撞检测
    void addCube(QVector3D relative_pos, QMatrix4x4 m_projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* m_program1, int type);
    void deleteCube(QVector3D relative_pos, QMatrix4x4 m_projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* m_program1);
    void clearVertexes();// 清除所有顶点
    void clearFaces();// 清除所有面
    void resetVisible();// 重置所有面为可视
};


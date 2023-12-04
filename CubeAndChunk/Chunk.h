#pragma once
#include <QVector>
#include <QVector3D>
#include "cubeList.h"


class Chunk
{
private:
    QVector<QVector<QVector<int>>> map;// 存储chunk内部每个位置的方块类型，三个维度分别对应x, y, z
    QVector3D pos;// 绘制起始位置
    int chunkSize;

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// 检查chunk内(x, y, z)位置的方块是否存在，若存在，相邻位置的方块不用绘制表面
    void buildChunk(QMatrix4x4 projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* program1);
    void setMap();
    void setPos(QVector3D ps);
    void setChunkSize(int size);

};


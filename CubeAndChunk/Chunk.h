#pragma once
#include <QVector>
#include <QVector3D>
#include <omp.h>
#include <vector>
#include "cubeList.h"
using namespace std;

class Chunk
{
private:
    QVector<QVector<QVector<int>>> map;// 存储chunk内部每个位置的方块类型，三个维度分别对应x, y, z
    QVector3D pos;// 绘制起始位置
    int chunkSize;
    vector<vector<vector<vector<bool>>>> visible_info;// (chunkSize,chunkSize,chunkSize,6)的矩阵，记录每个方块的每个面是否可视, true代表该面可视

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// 检查chunk内(x, y, z)位置的方块是否存在，若存在，相邻位置的方块不用绘制表面
    void buildChunk();
    void setMap();
    void setPos(QVector3D ps);
    void setChunkSize(int size);
    void calculateVisible();// 计算chunk内部每个方块的每个面的可视情况
    int getChunkSize();

};


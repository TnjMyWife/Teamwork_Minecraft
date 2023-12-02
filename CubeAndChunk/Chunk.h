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
    QVector<QVector<QVector<int>>> map;// �洢chunk�ڲ�ÿ��λ�õķ������ͣ�����ά�ȷֱ��Ӧx, y, z
    QVector3D pos;// ������ʼλ��
    int chunkSize;
    vector<vector<vector<vector<bool>>>> visible_info;// (chunkSize,chunkSize,chunkSize,6)�ľ��󣬼�¼ÿ�������ÿ�����Ƿ����, true����������

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// ���chunk��(x, y, z)λ�õķ����Ƿ���ڣ������ڣ�����λ�õķ��鲻�û��Ʊ���
    void buildChunk();
    void setMap();
    void setPos(QVector3D ps);
    void setChunkSize(int size);
    void calculateVisible();// ����chunk�ڲ�ÿ�������ÿ����Ŀ������
    int getChunkSize();

};


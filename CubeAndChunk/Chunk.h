#pragma once
#include <QVector>
#include <QVector3D>
#include "cubeList.h"


class Chunk
{
private:
    QVector<QVector<QVector<int>>> map;// �洢chunk�ڲ�ÿ��λ�õķ������ͣ�����ά�ȷֱ��Ӧx, y, z
    QVector3D pos;// ������ʼλ��
    int chunkSize;

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// ���chunk��(x, y, z)λ�õķ����Ƿ���ڣ������ڣ�����λ�õķ��鲻�û��Ʊ���
    void buildChunk(QMatrix4x4 projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* program1);
    void setMap();
    void setPos(QVector3D ps);
    void setChunkSize(int size);

};


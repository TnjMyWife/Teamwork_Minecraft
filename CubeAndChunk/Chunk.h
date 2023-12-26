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
} vertexInfo;// ������Ϣ��λ�á���ɫ���������꣩

typedef struct f {
    int type = -1;// �������ĸ�����
    int dir = 0;// ��ķ���
    vertexInfo v[4];
} face;

class Chunk
{
private:
    QVector<QVector<QVector<int>>> map;// �洢chunk�ڲ�ÿ��λ�õķ������ͣ�����ά�ȷֱ��Ӧx, y, z
    QVector3D pos;// ������ʼλ��
    int chunkSize;
    vector<vector<vector<vector<bool>>>> visible_info;// (chunkSize,chunkSize,chunkSize,6)�ľ��󣬼�¼ÿ�������ÿ�����Ƿ����, true����������
    float* arrVertex;// ����chunk�Ķ�������
    int numVertex;// arrVertex�еĶ�����
    int matrixUniform;
    QOpenGLBuffer vbo;
    QMatrix4x4 projectMat;
    QMatrix4x4 viewMat;
    QOpenGLShaderProgram* program1;
    QVector<face> arrFace;// ά����Ҫ���Ƶ��棬�����ϲ���arrVertex
    int numFace;// arrFace����ĸ���
    QVector<QVector<int>> typeDirTable;// ͳ��������ͺͳ���ı��
    QVector<QVector<int>> heightMap;// chunkSize*chunkSize, ��¼�߶���Ϣ

public:
    Chunk();
    ~Chunk();
    bool isCubeExist(int x, int y, int z);// ���chunk��(x, y, z)λ�õķ����Ƿ���ڣ������ڣ�����λ�õķ��鲻�û��Ʊ���
    void buildChunk();
    void setMap();
    void setPos(const float& x, const float& y, const float& z);// ����chunk(0, 0, 0)λ�÷��������
    QVector3D getPos();// ����pos
    void setChunkSize(int size);
    void calculateVisible();// ����chunk�ڲ�ÿ�������ÿ����Ŀ������
    int getChunkSize();
    void generateVertexArray();// ���ݵ�ͼ���ڵ�������ɶ�������
    void setChunk(QMatrix4x4 projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* program1, bool isInit = false);// ����Chunkǰ�ı�Ҫ׼��
    void addFace(vertexInfo v[], int type, int dir);// ���һ�������Ƶ��棬�ȴ��ϲ���arrVertex
    void initHeightMap();// �Ը߶���Ϣ���������ʼ��
    int valueNoise(int& i, int& j) const;// ����λ�ü�������
    float easeCurve(float& t) const;
    float lern(float& t, float& n1, float& n2) const;// �������Բ�ֵ
    void setCollision();// ����pos��map�����ײ���
    void addCube(QVector3D relative_pos, QMatrix4x4 m_projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* m_program1, int type);
    void deleteCube(QVector3D relative_pos, QMatrix4x4 m_projectMat, QMatrix4x4 viewMat, QOpenGLShaderProgram* m_program1);
    void clearVertexes();// ������ж���
    void clearFaces();// ���������
    void resetVisible();// ����������Ϊ����
};


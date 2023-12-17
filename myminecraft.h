#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <qgl.h>      

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QKeyEvent>
#include <QTimer>
#include <QMouseEvent>
#include "Camera/Camera.h"
#include "Character/character.h"
#include "CubeAndChunk/chunkList.h"
#include "change_cube/change_cube.h"

class MyGLWidget:public QOpenGLWidget, private QOpenGLFunctions_3_3_Core /*QOpenGLFunctions*/
{


public:
    MyGLWidget(QWidget* parent = nullptr, bool fs=false);
    ~MyGLWidget();



protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;


private:
    void createSharderProgram();
   // void makeOneCube();
    Cube* createCube(int cubeType);

private:
    QOpenGLShaderProgram* m_program1;
    QOpenGLContext* m_openGLContext;

    QOpenGLTexture* m_texture;
    QOpenGLBuffer m_vbo;
    int m_matrixUniform;
    QMatrix4x4 m_projectMat;

    bool fullscreen; 
    bool firstClick;
    float cubeSize;
    float yaw;
    float pitch;
    QTimer* timer;
    Camera camera;
    Character character;
    Cube* grass_block;
    Collision collision;
    change_cube changecube;
};

#endif // MYGLWIDGET_H
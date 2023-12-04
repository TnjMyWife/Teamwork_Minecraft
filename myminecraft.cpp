#include "myminecraft.h"
#include <QVariant>

MyGLWidget::MyGLWidget(QWidget* parent, bool fs):
    QOpenGLWidget(parent),
    m_program1(nullptr),
    m_texture(nullptr),
    m_vbo(QOpenGLBuffer::VertexBuffer),
    m_matrixUniform(0),
	m_projectMat(),
	firstClick(true),
	cubeSize(0.08f),
	yaw(0.0f),
	pitch(0.0f)
{
    fullscreen = fs;
    setGeometry(500, 500, 640, 480);               //���ô��ڴ�С��λ��
    setWindowTitle("mineCraft");  //���ô��ڱ���
    if (fullscreen) {
        showFullScreen();
    }
    setMouseTracking(true);      // ����������
    setCursor(Qt::BlankCursor);		// �������

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(16);
}

MyGLWidget::~MyGLWidget()
{
    m_vbo.destroy();
}

void MyGLWidget::initializeGL()
{

    initializeOpenGLFunctions();
    createSharderProgram();

	character.initCharacter(m_program1);
	grass_block = createCube(GRASS);
	glEnable(GL_TEXTURE_2D);    //��������
	// glEnable(GL_BLEND);				//��ɫ���
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glShadeModel(GL_SMOOTH);    //����smooth shading����Ӱƽ����

	glClearColor(0.29, 0.56, 0.9, 0.5);   //�����Ļʱ���õ���ɫ��rgba��0.0����ڣ�~1.0����������

	glClearDepth(1.0);  //������Ȼ���

	glEnable(GL_DEPTH_TEST);    //������Ȳ���
	glDepthFunc(GL_LEQUAL); //������Ȳ��Ե�����

	glEnable(GL_CULL_FACE);
}

void MyGLWidget::resizeGL(int w, int h)
{
    float aspect = float(w) / float(h ? h : 1);
    float fov = 45.0f, zNear = 0.041f, zFar = 50.0f;

    m_projectMat.setToIdentity();
    m_projectMat.perspective(fov, aspect, zNear, zFar);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����Ļ����Ȼ���
	

	int plainSize = 100;
	float spacing = 0.16;
	int layers = 10;
	// ����ƽԭ����ʼλ��
	float startX = -plainSize * spacing / 2.0;
	float startZ = -plainSize * spacing / 2.0;

	QVector3D cameraPos = camera.getCameraPos();
	QVector3D characterPos = camera.getCharacterPos();
	QVector3D cameraFront = camera.getCameraFront();
	QVector3D cameraUp = camera.getCameraUp();

    printf("%f,%f,%f\n", cameraPos.x(), cameraPos.y(), cameraPos.z());
    

	QMatrix4x4 viewMat = camera.getVewMat();			// �������任����

	// ��������
	character.drawCharacter(cameraFront, characterPos, m_projectMat * viewMat, m_program1);

	//���Ƶػ�
	grass_block->setCube(m_projectMat, viewMat, m_program1);
	grass_block->makeOneCube();


	for (int i = 0; i < layers; ++i)
	{
		for (int j = 0; j < plainSize; ++j)
		{
			for (int k = 0; k < plainSize; ++k)
			{
				// ����ÿ�������λ��
				float x = startX + k * spacing;
				float y = i * spacing; // ���
				float z = startZ + j * spacing;

				grass_block->drawCube(x, y, z);
			}
		}
	}

}

void MyGLWidget::createSharderProgram()
{
    m_program1 = new QOpenGLShaderProgram(this);
    m_program1->addShaderFromSourceFile(QOpenGLShader::Vertex, "vsharder.glsl");
    m_program1->addShaderFromSourceFile(QOpenGLShader::Fragment, "first_texture.glsl");
    m_program1->link();

    m_program1->bind();
    m_matrixUniform = m_program1->uniformLocation("mvpMat");

}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_W) {
		/* ��w��ʵ��ǰ�� */
		camera.moveForward();
		character.swing();
	}
	else if (e->key() == Qt::Key_S) {
		/* ��s��ʵ�ֺ��� */
		camera.moveBack();
		character.swing();
	}
	else if (e->key() == Qt::Key_A) {
		/* ��A��ʵ������ */
		camera.moveLeft();
		character.swing();
	}
	else if (e->key() == Qt::Key_D) {
		/* ��D��ʵ������ */
		camera.moveRight();
		character.swing();
	}
	else if (e->key() == Qt::Key_C) {
		/* C���� */
		camera.moveDown();
	}
	else if (e->key() == Qt::Key_Space) {
		/* �ո���Ծ */
		camera.moveUp();
	}
	else if (e->key() == Qt::Key_Q) {
		/* Qȫ�� */
		fullscreen = !fullscreen;
		if (fullscreen) {
			showFullScreen();
		}
		else {
			showNormal();
			setGeometry(500, 500, 640, 480);
		}
		update();
	}
	else if (e->key() == Qt::Key_Escape) {
		/* Esc�˳� */
		close();
	}
	else if (e->key() == Qt::Key_V) {
		/* V �����˳ƹ۲����� */
		camera.turnPerspect();
	}

}

void MyGLWidget::keyReleaseEvent(QKeyEvent* e) {
	if (!e->isAutoRepeat()) {
		character.setAngle(0.0f);
		return;
	}
}



float xpos;
float ypos;
void MyGLWidget::mouseMoveEvent(QMouseEvent* e)
{
	// ��������ƶ��¼�
	if (firstClick)
	{
		xpos = width() / 2.0;
		ypos = height() / 2.0;
		firstClick = false;
	}
	else
	{
		xpos = e->x();
		ypos = e->y();
	}

	float xOffset = width() / 2.0 - xpos;
	float yOffset = height() / 2.0 - ypos;
	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 80.0f)
		pitch = 79.0f;
	if (pitch < -60.0f)
		pitch = -59.0f;


	camera.updateCameraVectors(yaw, pitch);
	// ����������¶�λ����������
	QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

}

Cube* MyGLWidget::createCube(int cubeType) {
	QStringList imgs;
	Cube* block = NULL;
	switch (cubeType) {
	case GRASS:
		imgs << "texture/grass_block_top.png";
		imgs << "texture/grass_block_side.png";
		imgs << "texture/dirt.png";
		block = new Cube(cubeSize, imgs, GRASS);
		break;
	case DIRT:
		imgs << "texture/dirt.png";
		imgs << "texture/dirt.png";
		imgs << "texture/dirt.png";
		block = new Cube(cubeSize, imgs, DIRT);
		break;
	case STONE:
		imgs << "texture/texture/block/stone.png";
		imgs << "texture/texture/block/stone.png";
		imgs << "texture/texture/block/stone.png";
		block = new Cube(cubeSize, imgs, STONE);
		break;
	}
	return block;
}

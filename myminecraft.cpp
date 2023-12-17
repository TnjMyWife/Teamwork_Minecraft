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
	int cubeLen = cubeList.length(), chunkLen = chunkList.length();
	
	for (int i = 0; i < cubeLen; ++i) {
		delete cubeList[i];
	}
	
	for (int i = 0; i < chunkLen; ++i) {
		delete chunkList[i];
	}
    m_vbo.destroy();
}


void MyGLWidget::initializeGL()
{

    initializeOpenGLFunctions();
    createSharderProgram();

	character.initCharacter(m_program1);

	Cube* grass_cube = createCube(GRASS);
	cubeList.append(grass_cube);
	Cube* dirt_cube = createCube(DIRT);
	cubeList.append(dirt_cube);
	Cube* stone_cube = createCube(STONE);
	cubeList.append(stone_cube);
	cubeList[0]->setCube(m_projectMat, QMatrix4x4(), m_program1);
	cubeList[0]->makeOneCube(true);
	
	for (int i = 0; i < 1600; ++i) {
		Chunk* chunk0 = new Chunk();
		chunk0->setMap();
		chunk0->generateVertexArray();
		chunk0->setChunk(m_projectMat, QMatrix4x4(), m_program1, true);
		chunkList.append(chunk0);
	}
	int chunk_size = chunkList.at(0)->getChunkSize();
	int sqrt_chunk_num = 40;
	float spacing = 2 * cubeList.at(0)->getCubeSize();
	// ��������λ��
	for (int i = 0; i < sqrt_chunk_num; ++i) {
		for (int j = 0; j < sqrt_chunk_num; ++j) {
			chunkList[i * sqrt_chunk_num + j]->setPos(i * chunk_size * spacing, 0, j * chunk_size * spacing);
		}
	}
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
	

	int plainSize = 50;
	float spacing = 0.16;
	int layers = 1;
	// ����ƽԭ����ʼλ��
	float startX = -plainSize * spacing / 2.0;
	float startZ = -plainSize * spacing / 2.0;

	QVector3D cameraPos = camera.getCameraPos();
	QVector3D characterPos = camera.getCharacterPos();
	QVector3D cameraFront = camera.getCameraFront();
	QVector3D cameraUp = camera.getCameraUp();

    //printf("%f,%f,%f\n", cameraPos.x(), cameraPos.y(), cameraPos.z());
    

	QMatrix4x4 viewMat = camera.getVewMat();			// �������任����
	objects.clear();
	// ��������
	character.drawCharacter(cameraFront, characterPos, m_projectMat * viewMat, m_program1);

	//���Ƶػ�
	cubeList[0]->setCube(m_projectMat, viewMat, m_program1);
	cubeList[0]->makeOneCube();
	//����ӷ��鹤����ֻ��ɾ���ֶ���ӵķ���
	/*
	for (int i = 0; i < added_object.size(); i++) {
		float x = added_object[i].x(), y = added_object[i].y(), z = added_object[i].z();
		QString result = QString("X: %1, Y: %2, Z: %3")
			.arg(x)
			.arg(y)
			.arg(z);
		if (!deleted_object[result])
			cubeList[0]->drawCube(x, y, z);
	}
	*/
	//*/
	int chunk_size = chunkList.at(0)->getChunkSize();
	int sqrt_chunk_num = 40;
	float cube_size = cubeList[0]->getCubeSize();
	for (int i = 0; i < sqrt_chunk_num; ++i) {
		for (int j = 0; j < sqrt_chunk_num; ++j) {
			int id = i * sqrt_chunk_num + j;
			chunkList[id]->setChunk(m_projectMat, viewMat, m_program1);
			chunkList.at(id)->buildChunk();
			// ��������������Χ(+-cube_size)�����ظ��������ײ���
			QVector3D chunk_pos = chunkList.at(id)->getPos();
			if (cameraPos.x() >= chunk_pos.x() - cube_size * 2 &&
				cameraPos.x() < chunk_pos.x() + chunk_size * spacing &&
				cameraPos.z() >= chunk_pos.z() - cube_size * 2 &&
				cameraPos.z() < chunk_pos.z() + chunk_size * spacing) {
				chunkList[id]->setCollision();
			}
		}
	}
	/*/
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

				cubeList[0]->drawCube(x, y, z);
			}
		}
	}
	//*/
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
		camera.moveForward(collision);
		character.swing();
	}
	else if (e->key() == Qt::Key_S) {
		/* ��s��ʵ�ֺ��� */
		camera.moveBack(collision);
		character.swing();
	}
	else if (e->key() == Qt::Key_A) {
		/* ��A��ʵ������ */
		camera.moveLeft(collision);
		character.swing();
	}
	else if (e->key() == Qt::Key_D) {
		/* ��D��ʵ������ */
		camera.moveRight(collision);
		character.swing();
	}
	else if (e->key() == Qt::Key_C) {
		/* C���� */
		camera.moveDown(collision);
	}
	else if (e->key() == Qt::Key_Space) {
		/* �ո���Ծ */
		camera.moveUp(collision);
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
	else if (e->key() == Qt::Key_1) {
		changecube.tochange_cube(m_projectMat, m_program1, &camera, collision, ADD, 0);
		//update();
	}
	else if (e->key() == Qt::Key_2) {
		changecube.tochange_cube(m_projectMat, m_program1, &camera, collision, DELETE, 0);
		//update();
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
		imgs << "texture/stone.png";
		imgs << "texture/stone.png";
		imgs << "texture/stone.png";
		block = new Cube(cubeSize, imgs, STONE);
		break;
	}
	return block;
}

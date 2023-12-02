#include "myminecraft.h"

MyGLWidget::MyGLWidget(QWidget* parent, bool fs)
	: QOpenGLWidget(parent),
	cubeSize(0.08),
	yaw(0.0f),
	pitch(0.0f),
	firstClick(true)
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
	timer->start(0.1);
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
}


void MyGLWidget::initializeGL()
{
	Cube* grass_block = createCube(GRASS);
	cubeList.append(grass_block);

	Cube* dirt_block = createCube(DIRT);
	cubeList.append(dirt_block);

	Cube* stone_block = createCube(STONE);
	cubeList.append(stone_block);
	printf("===========chunk0==========\n");
	Chunk* chunk0 = new Chunk;
	chunk0->setPos(QVector3D(cubeSize, -21 * cubeSize, cubeSize));
	chunk0->setMap();
	chunk0->calculateVisible();
	chunkList.append(chunk0);
	printf("==========chunk1==========\n");
	Chunk* chunk1 = new Chunk;
	chunk1->setPos(QVector3D(cubeSize , -21 * cubeSize, -(chunk1->getChunkSize() - 1) * 2 * cubeSize + cubeSize));
	//chunk1->setPos(QVector3D(cubeSize, -21 * cubeSize, -30 * cubeSize + cubeSize));
	chunk1->setMap();
	chunk1->calculateVisible();
	chunkList.append(chunk1);
	printf("==========chunk2==========\n");
	Chunk* chunk2 = new Chunk;
	chunk2->setPos(QVector3D(-(chunk2->getChunkSize() - 1) * 2 * cubeSize + cubeSize, -21 * cubeSize, cubeSize));
	//chunk2->setPos(QVector3D(-30 * cubeSize + cubeSize, -21 * cubeSize, cubeSize));
	chunk2->setMap();
	chunk2->calculateVisible();
	chunkList.append(chunk2);
	printf("==========chunk3==========\n");
	Chunk* chunk3 = new Chunk;
	chunk3->setPos(QVector3D(-(chunk3->getChunkSize() - 1) * 2 * cubeSize + cubeSize, -21 * cubeSize, -(chunk3->getChunkSize() - 1) * 2 * cubeSize + cubeSize));
	//chunk3->setPos(QVector3D(-30 * cubeSize + cubeSize, -21 * cubeSize, -30 * cubeSize + cubeSize));
	chunk3->setMap();
	chunk3->calculateVisible();
	chunkList.append(chunk3);

	character.loadGLTextures();			// ���ؽ�ɫ����
	glEnable(GL_TEXTURE_2D);    //��������
	glEnable(GL_BLEND);				//��ɫ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);    //����smooth shading����Ӱƽ����

	glClearColor(0.29, 0.56, 0.9, 0.5);   //�����Ļʱ���õ���ɫ��rgba��0.0����ڣ�~1.0����������

	glClearDepth(1.0);  //������Ȼ���

	glEnable(GL_DEPTH_TEST);    //������Ȳ���
	glDepthFunc(GL_LEQUAL); //������Ȳ��Ե�����

	glEnable(GL_CULL_FACE);		// ���������޳�
	glCullFace(GL_BACK);		

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //������ϸ��͸������������OPenGL����ϣ��������õ�͸�����������ʮ����΢��Ӱ�����ܣ���ʹ��͸��ͼ��������һ��
}

void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����Ļ����Ȼ���

	//-----------------------------------------
	glLoadIdentity();   //���õ�ǰ��ģ�͹۲����

	// ����任
	camera.setCamera();
	QVector3D cameraPos = camera.getCameraPos();
	QVector3D characterPos = camera.getCharacterPos();
	QVector3D cameraFront = camera.getCameraFront();
	//printf("%f,%f,%f\n", cameraPos.x(), cameraPos.y(), cameraPos.z());

	objects.clear();		

	chunkList.at(0)->buildChunk();
	chunkList.at(1)->buildChunk();
	chunkList.at(2)->buildChunk();
	chunkList.at(3)->buildChunk();


	character.drawCharacter(cameraFront, characterPos);		// ��������

}

void MyGLWidget::resizeGL(int width, int height)
{
	if (height == 0) {    //��ֹhΪ0
		height = 1;
	}

	glViewport(0, 0, (GLint)width, (GLint)height);   //���õ�ǰ���ӿڣ�Viewport��

	glMatrixMode(GL_PROJECTION);    //ѡ��ͶӰ����

	glLoadIdentity();   //����ͶӰ����

	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.041, 100.0);  //����͸��ͶӰ����

	glMatrixMode(GL_MODELVIEW); //ѡ��ģ�͹۲����

	glLoadIdentity();   //����ģ�͹۲����
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
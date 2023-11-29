#include "myminecraft.h"

MyGLWidget::MyGLWidget(QWidget* parent, bool fs)
	: QGLWidget(parent), 
	cubeSize(0.08),
	yaw(0.0f),
	pitch(0.0f),
	firstClick(true)
{
	fullscreen = fs;
	setGeometry(500, 500, 640, 480);               //���ô��ڴ�С��λ��
	setWindowTitle("The first OpenGL Window");  //���ô��ڱ���
	if (fullscreen) {
		showFullScreen();
	}
	setMouseTracking(true);      // ����������
	setCursor(Qt::BlankCursor);		// �������

	// setFocusPolicy(Qt::StrongFocus);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
	timer->start(0.1);
}


MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	loadGLTextures();   //��������

	glEnable(GL_TEXTURE_2D);    //��������
	glEnable(GL_BLEND);				//��ɫ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);    //����smooth shading����Ӱƽ����

	glClearColor(0.29, 0.56, 0.9, 0.5);   //�����Ļʱ���õ���ɫ��rgba��0.0����ڣ�~1.0����������

	glClearDepth(1.0);  //������Ȼ���

	glEnable(GL_DEPTH_TEST);    //������Ȳ���
	glDepthFunc(GL_LEQUAL); //������Ȳ��Ե�����

	glEnable(GL_CULL_FACE);
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
	printf("%f,%f,%f\n", cameraPos.x(), cameraPos.y(), cameraPos.z());

	collision.objects.clear();		

	drawGrassCube(0.0f, cubeSize, 0.2f);
	drawPlain();			// ����ƽԭ
	character.drawCharacter(cameraFront, characterPos, texture);		// ��������

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
		updateGL();
	}
	else if (e->key() == Qt::Key_Escape) {
		/* Esc�˳� */
		close();
	}
	else if (e->key() == Qt::Key_C) {
		/* C �����˳ƹ۲����� */
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



void MyGLWidget::loadGLTextures()
{
	QStringList Images;
	Images << "texture/grass_block.png";
	Images << "texture/zombie.png";
	Images << "texture/fym.png";

	for (int i = 0; i < 3; ++i) {
		QImage tex, buf;
		if (!buf.load(Images.at(i))) {    //��QImage����������ͼƬ
			QImage dummy(128, 128, QImage::Format_RGB32); //������벻�ɹ�������һ��128*128��32λɫ����ɫͼƬ
			dummy.fill(Qt::green);
			buf = dummy;
		}
		tex = QGLWidget::convertToGLFormat(buf);    //QGLWidget�ľ�̬������ר������ת��ͼƬ

		glGenTextures(1, &texture[i]);   //����һ������

		glBindTexture(GL_TEXTURE_2D, texture[i]);    //ʹ������λͼ�������ɵĵ�������
		/*����OPenGL����������texture[0]�󶨵�����Ŀ���ϣ�2D����ֻ�и߶ȣ���Y���ϣ��Ϳ�ȣ���X���ϣ�*/

		//�����Ĵ�������
		//GL_TEXTURE_2D������ OpenGL ��������һ�� 2D ����
		//����0������ͼ�����ϸ�̶�;
		//����3�������ݵĳɷ�����
		//tex.width()��������Ŀ��
		//tex.height()��������ĸ߶ȣ�
		//GL_RGBA������ OpenGL ͼ�������ɺ졢�̡�����ɫ�����Լ� alpha ͨ��������ɣ�
		//GL_UNSIGNED_BYTE����ζ�����ͼ����������޷����ֽ����͵ģ�
		//tex.bits()������ OpenGL �������ݵ���Դ��
		// glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());


		//���� OpenGL ����ʾͼ��ʱ�������ȷŴ��ԭʼ�������GL_TEXTURE_MAG_FILTER������С�ñ�ԭʼ������С��GL_TEXTURE_MIN_FILTER��ʱ,
		//OpenGL ���õ��˲���ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//���Ƕ����� GL_LINEAR,��ʹ������Ӻ�Զ��������Ļ�ܽ�ʱ��ƽ����ʾ;
		//ʹ�� GL_LINEAR ��Ҫ CPU ���Կ������������,������Ļ�����������Ҳ��Ӧ�ò��� GL_NEAREST;
		//���˵������ڷŴ��ʱ�򣬿������߲��ĺ�,��Ҳ���Խ���������˲���ʽ: �ڽ���ʱʹ�� GL_LINEAR��Զ��ʱ GL_NEAREST;

		//����mipmap��������������Ҫ�� glTexImage2D���������������һ������Ϊ�ж���㼶����
		//gluBuild2DMipmaps ������������һ��forѭ�� ����glTexImage2D ������ͬ�㼶����
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); //����mipMap��С�㼶
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10); //����mipMap���㼶  ֻ�õ���4��

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//����������� GL_TEXTURE_WRAP_S  Ϊ GL_REPEAT ��ʾ����X����ѭ��ʹ������   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//����������� GL_TEXTURE_WRAP_T  Ϊ GL_MIRRORED_REPEAT ��ʾ����Y������ѭ��ʹ������


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

}


void MyGLWidget::drawGrassCube(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);  // ���÷����λ��
	//ѡ������ʹ�õ�����
	//����ڳ�����ʹ�ö������Ӧ��ʹ�� glBindTexture(GL_TEXTURE_2D, texture[��ʹ�������Ӧ������]) ѡ��Ҫ�󶨵�����;
	//���ı�����ʱ��Ӧ�ð��µ�����������������glBegin()��glEnd()֮�������������glBegin()֮ǰ��glEnd()֮���;
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//ǰ��
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(0.95 / 3.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(0.95 / 3.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0.95 / 3.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(0.95 / 3.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glEnd();

	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.05 / 3.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(1.05 / 3.0, 0.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glTexCoord2f(1.95 / 3.0, 0.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(1.95 / 3.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(2.05 / 3.0, 1.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(2.05 / 3.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.95 / 3.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(0.95 / 3.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(0.95 / 3.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(0.95 / 3.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glEnd();
	
	//���ƽ���
	glPopMatrix();

	QVector3D vector1 = QVector3D(x - cubeSize, y - cubeSize, z - cubeSize);
	QVector3D vector2 = QVector3D(x + cubeSize, y + cubeSize, z + cubeSize);
	collision.objects.push_back(object(vector1, vector2));
}

void MyGLWidget::drawPlain()
{
	int gridSize = 100;  // ���巽������Ĵ�С
	float spacing = cubeSize * 2;  // ���巽��֮��ļ��

	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			float x = i * spacing - (gridSize * spacing) / 2.0;
			float z = j * spacing - (gridSize * spacing) / 2.0;

			// ��ÿ������λ���ϻ��Ʒ���
			drawGrassCube(x, -cubeSize - 0.001, z);
		}
	}

}
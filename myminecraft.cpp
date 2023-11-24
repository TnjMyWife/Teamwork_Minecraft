#include "myminecraft.h"

MyGLWidget::MyGLWidget(QWidget* parent, bool fs)
	: QGLWidget(parent), 
	cubeSize(0.08),
	yaw(0.0f),
	pitch(0.0f),
	firstClick(true),
	cameraSpeed(0.08f)
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

	// ��������任
	handleCamera();
	printf("%f,%f,%f\n", cameraPos.x(), cameraPos.y(), cameraPos.z());

	drawGrassCube(0.0f, cubeSize, 0.2f);
	drawPlain();
	/*
	
	drawHead();
	drawBody();
	drawArm();
	drawLeg();
	*/


}

void MyGLWidget::resizeGL(int width, int height)
{
	if (height == 0) {    //��ֹhΪ0
		height = 1;
	}

	glViewport(0, 0, (GLint)width, (GLint)height);   //���õ�ǰ���ӿڣ�Viewport��

	glMatrixMode(GL_PROJECTION);    //ѡ��ͶӰ����

	glLoadIdentity();   //����ͶӰ����

	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);  //����͸��ͶӰ����

	glMatrixMode(GL_MODELVIEW); //ѡ��ģ�͹۲����

	glLoadIdentity();   //����ģ�͹۲����
}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_Q) {
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
		close();
	}
	else if (e->key() == Qt::Key_W) {
		/* ��w��ʵ��ǰ�� */
		cameraPos += cameraFront * cameraSpeed;
	}
	else if (e->key() == Qt::Key_S) {
		/* ��s��ʵ�ֺ��� */
		cameraPos -= cameraFront * cameraSpeed;
	}
	else if (e->key() == Qt::Key_A) {
		/* ��A��ʵ������ */
		cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
	}
	else if (e->key() == Qt::Key_D) {
		/* ��D��ʵ������ */
		cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
	}

	QWidget::keyPressEvent(e);

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

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCameraVectors();
	// ����������¶�λ����������
	QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

}


void MyGLWidget::loadGLTextures()
{
	QStringList Images;
	Images << "texture/grass_block_top.png";
	Images << "texture/grass_block_side.png";
	Images << "texture/dirt.png";

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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

}


void MyGLWidget::handleCamera()
{
	gluLookAt(cameraPos.x(), cameraPos.y(), cameraPos.z(),
		cameraPos.x() + cameraFront.x(), cameraPos.y() + cameraFront.y(), cameraPos.z() + cameraFront.z(),
		cameraUp.x(), cameraUp.y(), cameraUp.z());
}

void MyGLWidget::updateCameraVectors()
{
	QVector3D front, right, up;
	QVector3D worldUp = QVector3D(0.0f, 1.0f, 0.0f);
	front.setX(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
	front.setY(sin(qDegreesToRadians(pitch)));
	front.setZ(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
	cameraFront = front.normalized();
	right = QVector3D::crossProduct(cameraFront, worldUp).normalized();
	cameraUp = QVector3D::crossProduct(right, cameraFront).normalized();
}

void MyGLWidget::drawGrassCube(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);  // ���÷����λ��
	//ѡ������ʹ�õ�����
	//����ڳ�����ʹ�ö������Ӧ��ʹ�� glBindTexture(GL_TEXTURE_2D, texture[��ʹ�������Ӧ������]) ѡ��Ҫ�󶨵�����;
	//���ı�����ʱ��Ӧ�ð��µ�����������������glBegin()��glEnd()֮�������������glBegin()֮ǰ��glEnd()֮���;
	
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//ǰ��
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glEnd();
	
	//���ƽ���

	glPopMatrix();
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
			drawGrassCube(x, -0.5f, z);
		}
	}
}

void MyGLWidget::drawHead()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.28f, 0.0f);  // ���÷����λ��

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//ǰ��
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-0.04, -0.04, 0.04);
	glVertex3f(0.04, -0.04, 0.04);
	glVertex3f(0.04, 0.04, 0.04);
	glVertex3f(-0.04, 0.04, 0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-0.04, -0.04, -0.04);
	glVertex3f(-0.04, 0.04, -0.04);
	glVertex3f(0.04, 0.04, -0.04);
	glVertex3f(0.04, -0.04, -0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.04, 0.04, -0.04);
	glVertex3f(-0.04, 0.04, 0.04);
	glVertex3f(0.04, 0.04, 0.04);
	glVertex3f(0.04, 0.04, -0.04);
	glEnd();

	// glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.04, -0.04, -0.04);
	glVertex3f(0.04, -0.04, -0.04);
	glVertex3f(0.04, -0.04, 0.04);
	glVertex3f(-0.04, -0.04, 0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.04, -0.04, -0.04);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.04, 0.04, -0.04);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.04, 0.04, 0.04);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.04, -0.04, 0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.04, -0.04, -0.04);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.04, -0.04, 0.04);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.04, 0.04, 0.04);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.04, 0.04, -0.04);
	glEnd();

	//���ƽ���

	glPopMatrix();
}

void MyGLWidget::drawBody() {
	glPushMatrix();
	glTranslatef(0.0f, 0.18f, 0.0f);  // ���÷����λ��

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//ǰ��
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-0.04, -0.06, 0.02);
	glVertex3f(0.04, -0.06, 0.02);
	glVertex3f(0.04, 0.06, 0.02);
	glVertex3f(-0.04, 0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-0.04, -0.06, -0.02);
	glVertex3f(-0.04, 0.06, -0.02);
	glVertex3f(0.04, 0.06, -0.02);
	glVertex3f(0.04, -0.06, -0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.04, 0.06, -0.02);
	glVertex3f(-0.04, 0.06, 0.02);
	glVertex3f(0.04, 0.06, 0.02);
	glVertex3f(0.04, 0.06, -0.02);
	glEnd();

	// glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.04, -0.06, -0.02);
	glVertex3f(0.04, -0.06, -0.02);
	glVertex3f(0.04, -0.06, 0.02);
	glVertex3f(-0.04, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.04, -0.06, -0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.04, 0.06, -0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.04, 0.06, 0.02);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.04, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.04, -0.06, -0.02);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.04, -0.06, 0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.04, 0.06, 0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.04, 0.06, -0.02);
	glEnd();

	//���ƽ���

	glPopMatrix();
}

void MyGLWidget::drawOne() {
	

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//ǰ��
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-0.02, -0.06, 0.02);
	glVertex3f(0.02, -0.06, 0.02);
	glVertex3f(0.02, 0.06, 0.02);
	glVertex3f(-0.02, 0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-0.02, -0.06, -0.02);
	glVertex3f(-0.02, 0.06, -0.02);
	glVertex3f(0.02, 0.06, -0.02);
	glVertex3f(0.02, -0.06, -0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.02, 0.06, -0.02);
	glVertex3f(-0.02, 0.06, 0.02);
	glVertex3f(0.02, 0.06, 0.02);
	glVertex3f(0.02, 0.06, -0.02);
	glEnd();

	// glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.02, -0.06, -0.02);
	glVertex3f(0.02, -0.06, -0.02);
	glVertex3f(0.02, -0.06, 0.02);
	glVertex3f(-0.02, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.02, -0.06, -0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.02, 0.06, -0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.02, 0.06, 0.02);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.02, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//����
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.02, -0.06, -0.02);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.02, -0.06, 0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.02, 0.06, 0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.02, 0.06, -0.02);
	glEnd();

	//���ƽ���

	
}

void MyGLWidget::drawArm() {
	glPushMatrix();
	glTranslatef(0.06f, 0.18f, 0.0f);  // ���÷����λ��
	drawOne();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.06f, 0.18f, 0.0f);  // ���÷����λ��
	drawOne();
	glPopMatrix();

}

void MyGLWidget::drawLeg() {
	glPushMatrix();
	glTranslatef(0.02f, 0.06f, 0.0f);  // ���÷����λ��
	drawOne();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.02f, 0.06f, 0.0f);  // ���÷����λ��
	drawOne();
	glPopMatrix();
}



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
	setGeometry(500, 500, 640, 480);               //设置窗口大小、位置
	setWindowTitle("The first OpenGL Window");  //设置窗口标题
	if (fullscreen) {
		showFullScreen();
	}
	setMouseTracking(true);      // 开启鼠标跟踪
	setCursor(Qt::BlankCursor);		// 隐藏鼠标

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
	loadGLTextures();   //载入纹理

	glEnable(GL_TEXTURE_2D);    //启用纹理
	glEnable(GL_BLEND);				//颜色混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);    //启用smooth shading（阴影平滑）

	glClearColor(0.29, 0.56, 0.9, 0.5);   //清除屏幕时所用的颜色，rgba【0.0（最黑）~1.0（最亮）】

	glClearDepth(1.0);  //设置深度缓存

	glEnable(GL_DEPTH_TEST);    //启动深度测试
	glDepthFunc(GL_LEQUAL); //所作深度测试的类型

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //真正精细的透视修正，告诉OPenGL我们希望进行最好的透视修正，这会十分轻微的影响性能，但使得透视图看起来好一点


}

void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存

	//-----------------------------------------
	glLoadIdentity();   //重置当前的模型观察矩阵

	// 处理相机变换
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
	if (height == 0) {    //防止h为0
		height = 1;
	}

	glViewport(0, 0, (GLint)width, (GLint)height);   //重置当前的视口（Viewport）

	glMatrixMode(GL_PROJECTION);    //选择投影矩阵

	glLoadIdentity();   //重置投影矩阵

	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);  //建立透视投影矩阵

	glMatrixMode(GL_MODELVIEW); //选择模型观察矩阵

	glLoadIdentity();   //重置模型观察矩阵
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
		/* 按w键实现前进 */
		cameraPos += cameraFront * cameraSpeed;
	}
	else if (e->key() == Qt::Key_S) {
		/* 按s键实现后退 */
		cameraPos -= cameraFront * cameraSpeed;
	}
	else if (e->key() == Qt::Key_A) {
		/* 按A键实现向左 */
		cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
	}
	else if (e->key() == Qt::Key_D) {
		/* 按D键实现向右 */
		cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
	}

	QWidget::keyPressEvent(e);

}

float xpos;
float ypos;
void MyGLWidget::mouseMoveEvent(QMouseEvent* e)
{
	// 处理鼠标移动事件
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
	// 将鼠标光标重新定位到窗口中心
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
		if (!buf.load(Images.at(i))) {    //用QImage类载入纹理图片
			QImage dummy(128, 128, QImage::Format_RGB32); //如果载入不成功，生成一个128*128的32位色的绿色图片
			dummy.fill(Qt::green);
			buf = dummy;
		}
		tex = QGLWidget::convertToGLFormat(buf);    //QGLWidget的静态函数，专门用来转换图片

		glGenTextures(1, &texture[i]);   //创建一个纹理

		glBindTexture(GL_TEXTURE_2D, texture[i]);    //使用来自位图数据生成的典型纹理
		/*告诉OPenGL将纹理名字texture[0]绑定到纹理目标上；2D纹理只有高度（在Y轴上）和宽度（在X轴上）*/

		//真正的创建纹理
		//GL_TEXTURE_2D：告诉 OpenGL 此纹理是一个 2D 纹理；
		//数字0：代表图像的详细程度;
		//数字3：是数据的成分数；
		//tex.width()：是纹理的宽度
		//tex.height()：是纹理的高度；
		//GL_RGBA：告诉 OpenGL 图像数据由红、绿、蓝三色数据以及 alpha 通道数据组成；
		//GL_UNSIGNED_BYTE：意味着组成图像的数据是无符号字节类型的；
		//tex.bits()：告诉 OpenGL 纹理数据的来源；
		// glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());


		//告诉 OpenGL 在显示图像时，当它比放大得原始的纹理大（GL_TEXTURE_MAG_FILTER）或缩小得比原始得纹理小（GL_TEXTURE_MIN_FILTER）时,
		//OpenGL 采用的滤波方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//我们都采用 GL_LINEAR,这使得纹理从很远处到离屏幕很近时都平滑显示;
		//使用 GL_LINEAR 需要 CPU 和显卡做更多的运算,如果您的机器很慢，您也许应该采用 GL_NEAREST;
		//过滤的纹理在放大的时候，看起来斑驳的很,您也可以结合这两种滤波方式: 在近处时使用 GL_LINEAR，远处时 GL_NEAREST;

		//创建mipmap纹理，产生的纹理要比 glTexImage2D函数产生的纹理大一倍，因为有多个层级纹理
		//gluBuild2DMipmaps 函数本质上是一个for循环 调用glTexImage2D 产生不同层级纹理
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); //设置mipMap最小层级
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10); //设置mipMap最大层级  只用到第4级

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
	glTranslatef(x, y, z);  // 设置方块的位置
	//选择我们使用的纹理
	//如果在场景中使用多个纹理，应该使用 glBindTexture(GL_TEXTURE_2D, texture[所使用纹理对应的数字]) 选择要绑定的纹理;
	//当改变纹理时，应该绑定新的纹理。并且您不能在glBegin()和glEnd()之间绑定纹理，必须在glBegin()之前或glEnd()之后绑定;
	
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//后面
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//顶面
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//底面
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//右面
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//左面
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
	glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
	glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
	glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
	glEnd();
	
	//绘制结束

	glPopMatrix();
}

void MyGLWidget::drawPlain()
{
	int gridSize = 100;  // 定义方块网格的大小
	float spacing = cubeSize * 2;  // 定义方块之间的间距

	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			float x = i * spacing - (gridSize * spacing) / 2.0;
			float z = j * spacing - (gridSize * spacing) / 2.0;

			// 在每个网格位置上绘制方块
			drawGrassCube(x, -0.5f, z);
		}
	}
}

void MyGLWidget::drawHead()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.28f, 0.0f);  // 设置方块的位置

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-0.04, -0.04, 0.04);
	glVertex3f(0.04, -0.04, 0.04);
	glVertex3f(0.04, 0.04, 0.04);
	glVertex3f(-0.04, 0.04, 0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//后面
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-0.04, -0.04, -0.04);
	glVertex3f(-0.04, 0.04, -0.04);
	glVertex3f(0.04, 0.04, -0.04);
	glVertex3f(0.04, -0.04, -0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//顶面
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.04, 0.04, -0.04);
	glVertex3f(-0.04, 0.04, 0.04);
	glVertex3f(0.04, 0.04, 0.04);
	glVertex3f(0.04, 0.04, -0.04);
	glEnd();

	// glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//底面
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.04, -0.04, -0.04);
	glVertex3f(0.04, -0.04, -0.04);
	glVertex3f(0.04, -0.04, 0.04);
	glVertex3f(-0.04, -0.04, 0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//右面
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.04, -0.04, -0.04);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.04, 0.04, -0.04);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.04, 0.04, 0.04);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.04, -0.04, 0.04);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//左面
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.04, -0.04, -0.04);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.04, -0.04, 0.04);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.04, 0.04, 0.04);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.04, 0.04, -0.04);
	glEnd();

	//绘制结束

	glPopMatrix();
}

void MyGLWidget::drawBody() {
	glPushMatrix();
	glTranslatef(0.0f, 0.18f, 0.0f);  // 设置方块的位置

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-0.04, -0.06, 0.02);
	glVertex3f(0.04, -0.06, 0.02);
	glVertex3f(0.04, 0.06, 0.02);
	glVertex3f(-0.04, 0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//后面
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-0.04, -0.06, -0.02);
	glVertex3f(-0.04, 0.06, -0.02);
	glVertex3f(0.04, 0.06, -0.02);
	glVertex3f(0.04, -0.06, -0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//顶面
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.04, 0.06, -0.02);
	glVertex3f(-0.04, 0.06, 0.02);
	glVertex3f(0.04, 0.06, 0.02);
	glVertex3f(0.04, 0.06, -0.02);
	glEnd();

	// glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//底面
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.04, -0.06, -0.02);
	glVertex3f(0.04, -0.06, -0.02);
	glVertex3f(0.04, -0.06, 0.02);
	glVertex3f(-0.04, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//右面
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.04, -0.06, -0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.04, 0.06, -0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.04, 0.06, 0.02);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.04, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//左面
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.04, -0.06, -0.02);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.04, -0.06, 0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.04, 0.06, 0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.04, 0.06, -0.02);
	glEnd();

	//绘制结束

	glPopMatrix();
}

void MyGLWidget::drawOne() {
	

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-0.02, -0.06, 0.02);
	glVertex3f(0.02, -0.06, 0.02);
	glVertex3f(0.02, 0.06, 0.02);
	glVertex3f(-0.02, 0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//后面
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-0.02, -0.06, -0.02);
	glVertex3f(-0.02, 0.06, -0.02);
	glVertex3f(0.02, 0.06, -0.02);
	glVertex3f(0.02, -0.06, -0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//顶面
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.02, 0.06, -0.02);
	glVertex3f(-0.02, 0.06, 0.02);
	glVertex3f(0.02, 0.06, 0.02);
	glVertex3f(0.02, 0.06, -0.02);
	glEnd();

	// glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//底面
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.02, -0.06, -0.02);
	glVertex3f(0.02, -0.06, -0.02);
	glVertex3f(0.02, -0.06, 0.02);
	glVertex3f(-0.02, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//右面
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.02, -0.06, -0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.02, 0.06, -0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.02, 0.06, 0.02);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.02, -0.06, 0.02);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//左面
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.02, -0.06, -0.02);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.02, -0.06, 0.02);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.02, 0.06, 0.02);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.02, 0.06, -0.02);
	glEnd();

	//绘制结束

	
}

void MyGLWidget::drawArm() {
	glPushMatrix();
	glTranslatef(0.06f, 0.18f, 0.0f);  // 设置方块的位置
	drawOne();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.06f, 0.18f, 0.0f);  // 设置方块的位置
	drawOne();
	glPopMatrix();

}

void MyGLWidget::drawLeg() {
	glPushMatrix();
	glTranslatef(0.02f, 0.06f, 0.0f);  // 设置方块的位置
	drawOne();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.02f, 0.06f, 0.0f);  // 设置方块的位置
	drawOne();
	glPopMatrix();
}



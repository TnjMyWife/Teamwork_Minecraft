#include "Cube.h"

Cube::Cube(float size, QStringList& Images, int type) {
	/*
	初始化方块的大小和贴图
	size：边长的1/2
	Images：贴图，top、side、bottom
	*/
	resetVisible();
	cubeSize = size;
	cubeType = type;
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//设置纹理参数 GL_TEXTURE_WRAP_S  为 GL_REPEAT 表示纹理X方向循环使用纹理   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//设置纹理参数 GL_TEXTURE_WRAP_T  为 GL_MIRRORED_REPEAT 表示纹理Y方向镜像循环使用纹理


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

}

Cube::~Cube() {

}

void Cube::drawCube(float x, float y, float z) {

	glPushMatrix();
	glTranslatef(x, y, z);  // 设置方块的位置
	//选择我们使用的纹理
	//如果在场景中使用多个纹理，应该使用 glBindTexture(GL_TEXTURE_2D, texture[所使用纹理对应的数字]) 选择要绑定的纹理;
	//当改变纹理时，应该绑定新的纹理。并且您不能在glBegin()和glEnd()之间绑定纹理，必须在glBegin()之前或glEnd()之后绑定;

	// top
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	if (cubeType == GRASS)
		glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[TOP]) {
		glBegin(GL_QUADS);
		//顶面
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);// 贴图左下
		glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, cubeSize, cubeSize);// 贴图右下
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);// 贴图右上
		glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);// 贴图左上
		glEnd();
	}

	// side
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[FRONT]) {
		glBegin(GL_QUADS);
		//前面，向z轴负方向看为逆时针
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);// 左下
		glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);// 右下
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);// 右上
		glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);// 左上
		glEnd();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[BACK]) {
		glBegin(GL_QUADS);
		//后面，向z轴负方向看为顺时针
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);// 左下
		glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);// 左上
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);// 右上
		glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);// 右下
		glEnd();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[RIGHT]) {
		glBegin(GL_QUADS);
		//右面
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);
		glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
		glEnd();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[LEFT]) {
		glBegin(GL_QUADS);
		//左面
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
		glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);
		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[BOTTOM]) {
		glBegin(GL_QUADS);
		//底面
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
		glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
		glEnd();
	}
	//绘制结束

	glPopMatrix();

	QVector3D vector1 = QVector3D(x - cubeSize, y - cubeSize, z - cubeSize);
	QVector3D vector2 = QVector3D(x + cubeSize, y + cubeSize, z + cubeSize);
	objects.push_back(object(vector1, vector2));

}

void Cube::setInvisible(int i) {
	isVisible[i] = false;
}

void Cube::resetVisible() {
	for (int i = 0; i < 6; ++i) {
		isVisible[i] = true;
	}
}

float Cube::getCubeSize() {
	return cubeSize;
}
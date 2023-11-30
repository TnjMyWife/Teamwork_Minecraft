#include "Character.h"

Character::Character():
	angle(0.0f),
	swingSpeed(10.0f)
{
	
}

Character::~Character()
{

}


void Character::setAngle(float angle)
{
	this->angle = angle;
}

void Character::swing()
{
	angle += swingSpeed;
	if (angle >= 45.0f || angle <= -45.0f) {
		swingSpeed = -swingSpeed; // 反转旋转方向
	}
}

void Character::drawHead(QVector3D cameraFront)
{
	glPushMatrix();
	glTranslatef(0.0f, 0.28f, 0.0f);  // 设置头部的初始位置


	/* 头部随鼠标转动 */
	QVector3D rotationAxis = QVector3D::crossProduct(QVector3D(0.0f, 0.0f, 1.0f), cameraFront);			/* 因为初始朝向为(0,0,1), 计算相机方向与z轴叉乘，得到旋转轴 */
	qreal rotationAngle = qRadiansToDegrees(qAcos(QVector3D::dotProduct(QVector3D(0.0f, 0.0f, 1.0f), cameraFront)));	/* 计算旋转角度 */
	glRotatef(rotationAngle, rotationAxis.x(), rotationAxis.y(), rotationAxis.z());			/* 旋转 */


	/* 头部绘制 */
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(8.0f / 56.0f, 16.0f / 32.0f); glVertex3f(-0.04, -0.04, 0.04);
	glTexCoord2f(16.0f / 56.0f, 16.0f / 32.0f); glVertex3f(0.04, -0.04, 0.04);
	glTexCoord2f(16.0f / 56.0f, 24.0f / 32.0f); glVertex3f(0.04, 0.04, 0.04);
	glTexCoord2f(8.0f / 56.0f, 24.0f / 32.0f); glVertex3f(-0.04, 0.04, 0.04);
	glEnd();
	/*
	// fym专属
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.04, -0.04, 0.04);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.04, -0.04, 0.04);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.04, 0.04, 0.04);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.04, 0.04, 0.04);
	glEnd();
	*/


	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//后面
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(24.0f / 56.0f, 16.0f / 32.0f); glVertex3f(-0.04, -0.04, -0.04);
	glTexCoord2f(24.0f / 56.0f, 24.0f / 32.0f); glVertex3f(-0.04, 0.04, -0.04);
	glTexCoord2f(31.0f / 56.0f, 24.0f / 32.0f); glVertex3f(0.04, 0.04, -0.04);
	glTexCoord2f(31.0f / 56.0f, 16.0f / 32.0f); glVertex3f(0.04, -0.04, -0.04);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//顶面
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 16.0f / 32.0f); glVertex3f(-0.04, 0.04, -0.04);
	glTexCoord2f(8.0f / 56.0f, 16.0f / 32.0f); glVertex3f(-0.04, 0.04, 0.04);
	glTexCoord2f(8.0f / 56.0f, 24.0f / 32.0f); glVertex3f(0.04, 0.04, 0.04);
	glTexCoord2f(0.0f, 24.0f / 32.0f); glVertex3f(0.04, 0.04, -0.04);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//底面
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(17.0f / 56.0f, 32.0f / 32.0f); glVertex3f(-0.04, -0.04, -0.04);
	glTexCoord2f(23.0f / 56.0f, 32.0f / 32.0f); glVertex3f(0.04, -0.04, -0.04);
	glTexCoord2f(23.0f / 56.0f, 24.0f / 32.0f); glVertex3f(0.04, -0.04, 0.04);
	glTexCoord2f(17.0f / 56.0f, 24.0f / 32.0f); glVertex3f(-0.04, -0.04, 0.04);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//右面
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(24.0f / 56.0f, 16.0f / 32.0f); glVertex3f(0.04, -0.04, -0.04);
	glTexCoord2f(24.0f / 56.0f, 24.0f / 32.0f); glVertex3f(0.04, 0.04, -0.04);
	glTexCoord2f(16.0f / 56.0f, 24.0f / 32.0f); glVertex3f(0.04, 0.04, 0.04);
	glTexCoord2f(16.0f / 56.0f, 16.0f / 32.0f); glVertex3f(0.04, -0.04, 0.04);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//左面
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0f, 16.0f / 32.0f); glVertex3f(-0.04, -0.04, -0.04);
	glTexCoord2f(8.0f / 56.0f, 16.0f / 32.0f); glVertex3f(-0.04, -0.04, 0.04);
	glTexCoord2f(8.0f / 56.0f, 24.0f / 32.0f); glVertex3f(-0.04, 0.04, 0.04);
	glTexCoord2f(0.0f, 24.0f / 32.0f); glVertex3f(-0.04, 0.04, -0.04);
	glEnd();

	glPopMatrix();
}

void Character::drawBody()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.18f, 0.0f);  /* 设置身体的位置 */


	/* 绘制身体 */
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(20.0f / 56.0f, 0.0f / 32.0f); glVertex3f(-0.04, -0.06, 0.02);
	glTexCoord2f(28.0f / 56.0f, 0.0f / 32.0f); glVertex3f(0.04, -0.06, 0.02);
	glTexCoord2f(28.0f / 56.0f, 12.0f / 32.0f); glVertex3f(0.04, 0.06, 0.02);
	glTexCoord2f(20.0f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.04, 0.06, 0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//后面
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(40.0f / 56.0f, 0.0f / 32.0f); glVertex3f(-0.04, -0.06, -0.02);
	glTexCoord2f(40.0f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.04, 0.06, -0.02);
	glTexCoord2f(32.0f / 56.0f, 12.0f / 32.0f); glVertex3f(0.04, 0.06, -0.02);
	glTexCoord2f(32.0f / 56.0f, 0.0f / 32.0f); glVertex3f(0.04, -0.06, -0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//顶面
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(20.1f / 56.0f, 16.0f / 32.0f); glVertex3f(-0.04, 0.06, -0.02);
	glTexCoord2f(20.1f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.04, 0.06, 0.02);
	glTexCoord2f(28.0f / 56.0f, 12.0f / 32.0f); glVertex3f(0.04, 0.06, 0.02);
	glTexCoord2f(28.0f / 56.0f, 16.0f / 32.0f); glVertex3f(0.04, 0.06, -0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//底面
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(28.0f / 56.0f, 16.0 / 32.0f); glVertex3f(-0.04, -0.06, -0.02);
	glTexCoord2f(35.5f / 56.0f, 16.0 / 32.0f); glVertex3f(0.04, -0.06, -0.02);
	glTexCoord2f(35.5f / 56.0f, 12.0 / 32.0f); glVertex3f(0.04, -0.06, 0.02);
	glTexCoord2f(28.0f / 56.0f, 12.0 / 32.0f); glVertex3f(-0.04, -0.06, 0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//右面
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(20.0f / 56.0f, 0.0); glVertex3f(0.04, -0.06, -0.02);
	glTexCoord2f(20.0f / 56.0f, 12.0f / 32.0f); glVertex3f(0.04, 0.06, -0.02);
	glTexCoord2f(16.0f / 56.0f, 12.0f / 32.0f); glVertex3f(0.04, 0.06, 0.02);
	glTexCoord2f(16.0f / 56.0f, 0.0); glVertex3f(0.04, -0.06, 0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//左面
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(16.0f / 56.0f, 0.0); glVertex3f(-0.04, -0.06, -0.02);
	glTexCoord2f(20.0f / 56.0f, 0.0); glVertex3f(-0.04, -0.06, 0.02);
	glTexCoord2f(20.0f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.04, 0.06, 0.02);
	glTexCoord2f(16.0f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.04, 0.06, -0.02);
	glEnd();

	glPopMatrix();
}

void Character::drawOne()
{
	/* 绘制一条四肢*/
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//前面
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(4.0f / 56.0f, 0.0f / 32.0f); glVertex3f(-0.02, -0.06, 0.02);
	glTexCoord2f(8.0f / 56.0f, 0.0f / 32.0f); glVertex3f(0.02, -0.06, 0.02);
	glTexCoord2f(8.0f / 56.0f, 12.0f / 32.0f); glVertex3f(0.02, 0.06, 0.02);
	glTexCoord2f(4.0f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.02, 0.06, 0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//后面
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(15.5f / 56.0f, 0.0f / 32.0f); glVertex3f(-0.02, -0.06, -0.02);
	glTexCoord2f(15.5f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.02, 0.06, -0.02);
	glTexCoord2f(12.0f / 56.0f, 12.0f / 32.0f); glVertex3f(0.02, 0.06, -0.02);
	glTexCoord2f(12.0f / 56.0f, 0.0f / 32.0f); glVertex3f(0.02, -0.06, -0.02);
	glEnd();

	glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	glBegin(GL_QUADS);
	//顶面
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.02, 0.06, -0.02);
	glVertex3f(-0.02, 0.06, 0.02);
	glVertex3f(0.02, 0.06, 0.02);
	glVertex3f(0.02, 0.06, -0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//底面
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.02, -0.06, -0.02);
	glVertex3f(0.02, -0.06, -0.02);
	glVertex3f(0.02, -0.06, 0.02);
	glVertex3f(-0.02, -0.06, 0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//右面
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(11.5f / 56.0f, 0.0f / 32.0f); glVertex3f(0.02, -0.06, -0.02);
	glTexCoord2f(11.5f / 56.0f, 12.0f / 32.0f); glVertex3f(0.02, 0.06, -0.02);
	glTexCoord2f(8.5f / 56.0f, 12.0f / 32.0f); glVertex3f(0.02, 0.06, 0.02);
	glTexCoord2f(8.5f / 56.0f, 0.0f / 32.0f); glVertex3f(0.02, -0.06, 0.02);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//左面
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0f / 56.0f, 0.0f / 32.0f); glVertex3f(-0.02, -0.06, -0.02);
	glTexCoord2f(3.5f / 56.0f, 0.0f / 32.0f); glVertex3f(-0.02, -0.06, 0.02);
	glTexCoord2f(3.5f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.02, 0.06, 0.02);
	glTexCoord2f(0.5f / 56.0f, 12.0f / 32.0f); glVertex3f(-0.02, 0.06, -0.02);
	glEnd();

	//绘制结束

}

void Character::drawArm() {
	glPushMatrix();
	glTranslatef(-0.06f, 0.22f, 0.0f);  // 设置手臂初始位置
	glRotatef(angle, 1.0f, 0.0f, 0.0f);	// 绕x轴旋转，angle在移动事件会进行实时更新
	glTranslatef(0.0f, -0.04f, 0.0f);  // 先下移0.04
	drawOne();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.06f, 0.22f, 0.0f);  // 设置设置手臂初始位置
	glRotatef(-angle, 1.0f, 0.0f, 0.0f);	// 绕x轴旋转，angle在移动事件会进行实时更新
	glTranslatef(0.0f, -0.04f, 0.0f);  // 先下移0.04
	drawOne();
	glPopMatrix();

}

void Character::drawLeg() {
	glPushMatrix();
	glTranslatef(0.02f, 0.12f, 0.0f);  // 设置腿初始位置
	glRotatef(angle, 1.0f, 0.0f, 0.0f);	// 绕x轴旋转，angle在移动事件会进行实时更新
	glTranslatef(0.0f, -0.06f, 0.0f);  // 先下移0.06
	drawOne();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.02f, 0.12f, 0.0f);  // 设置腿初始位置
	glRotatef(-angle, 1.0f, 0.0f, 0.0f);	// 绕x轴旋转，angle在移动事件会进行实时更新
	glTranslatef(0.0f, -0.06f, 0.0f);  // 先下移0.06
	drawOne();
	glPopMatrix();
}

void Character::drawCharacter(QVector3D cameraFront, QVector3D characterPos)
{
	glPushMatrix();
	glTranslatef(characterPos.x(), characterPos.y(), characterPos.z());  // 设置人物的位置，这个位置随着移动而变化（同相机）

	drawHead(cameraFront);

	/* 身体随着视角转动 */
	QVector3D xoyCameraFront = QVector3D(cameraFront.x(), 0.0f, cameraFront.z());		// 相机方向向量投影到xoz平面
	QVector3D rotationAxis = QVector3D::crossProduct(QVector3D(0.0f, 0.0f, 1.0f), xoyCameraFront);		// 与z轴叉乘得到旋转轴（此处实际上得到为y轴）
	qreal rotationAngle = qRadiansToDegrees(qAcos(QVector3D::dotProduct(QVector3D(0.0f, 0.0f, 1.0f), xoyCameraFront)));		// 旋转角度计算
	glRotatef(rotationAngle, rotationAxis.x(), rotationAxis.y(), rotationAxis.z());

	drawBody();
	drawArm();
	drawLeg();
	glPopMatrix();
}

void Character::loadGLTextures()
{
	QStringList Images;
	Images << "texture/grass_block.png";
	Images << "texture/zombie.png";
	Images << "texture/fym.png";

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
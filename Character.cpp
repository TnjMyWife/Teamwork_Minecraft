#include <Character.h>

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

void Character::drawHead(QVector3D cameraFront, GLuint* texture)
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

void Character::drawCharacter(QVector3D cameraFront, QVector3D characterPos, GLuint* texture)
{
	glPushMatrix();
	glTranslatef(characterPos.x(), characterPos.y(), characterPos.z());  // 设置人物的位置，这个位置随着移动而变化（同相机）

	drawHead(cameraFront, texture);

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


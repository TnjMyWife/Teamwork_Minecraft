#include "Cube.h"

Cube::Cube(float size, QStringList& Images, int type) {
	/*
	��ʼ������Ĵ�С����ͼ
	size���߳���1/2
	Images����ͼ��top��side��bottom
	*/
	resetVisible();
	cubeSize = size;
	cubeType = type;
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

Cube::~Cube() {

}

void Cube::drawCube(float x, float y, float z) {

	glPushMatrix();
	glTranslatef(x, y, z);  // ���÷����λ��
	//ѡ������ʹ�õ�����
	//����ڳ�����ʹ�ö������Ӧ��ʹ�� glBindTexture(GL_TEXTURE_2D, texture[��ʹ�������Ӧ������]) ѡ��Ҫ�󶨵�����;
	//���ı�����ʱ��Ӧ�ð��µ�����������������glBegin()��glEnd()֮�������������glBegin()֮ǰ��glEnd()֮���;

	// top
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	if (cubeType == GRASS)
		glColor4f(0.6f, 1.0f, 0.35f, 1.0f);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[TOP]) {
		glBegin(GL_QUADS);
		//����
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);// ��ͼ����
		glTexCoord2f(1.0, 0.0); glVertex3f(-cubeSize, cubeSize, cubeSize);// ��ͼ����
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);// ��ͼ����
		glTexCoord2f(0.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);// ��ͼ����
		glEnd();
	}

	// side
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[FRONT]) {
		glBegin(GL_QUADS);
		//ǰ�棬��z�Ḻ����Ϊ��ʱ��
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);// ����
		glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);// ����
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, cubeSize);// ����
		glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, cubeSize);// ����
		glEnd();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[BACK]) {
		glBegin(GL_QUADS);
		//���棬��z�Ḻ����Ϊ˳ʱ��
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);// ����
		glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, cubeSize, -cubeSize);// ����
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, cubeSize, -cubeSize);// ����
		glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);// ����
		glEnd();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (isVisible[RIGHT]) {
		glBegin(GL_QUADS);
		//����
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
		//����
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
		//����
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-cubeSize, -cubeSize, cubeSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(-cubeSize, -cubeSize, -cubeSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(cubeSize, -cubeSize, -cubeSize);
		glTexCoord2f(1.0, 0.0); glVertex3f(cubeSize, -cubeSize, cubeSize);
		glEnd();
	}
	//���ƽ���

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
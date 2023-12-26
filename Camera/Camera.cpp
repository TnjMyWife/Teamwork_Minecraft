#include "Camera.h"

Camera::Camera() :
	firstPerspect(true),
	cameraSpeed(0.05f),	
	cameraPos(0.0f, 0.28f, 0.0f),	// ���λ�ã���һ�˳���Ҫ̧��ͷ��λ�ã���ʼ��Ϊ(0.0f, 0.28f, 0.0f)��
									//α�����˳�ʱ����Ҫ�����̧�ñ�0.28�ߣ������������(0.0f, 0.4f, -1.0f)��������Z������
	targetPos(0.0f, 0.0f, 0.0f),
	characterPos(0.0f, 0.0f, 0.0f),		// ������Գ�ʼͷ��(0.0, 0.28, 0.0)�ƶ���λ��,���øĶ�
	cameraFront(0.0f, 0.0f, -1.0f),		// �������ķ���
	cameraUp(0.0f, 1.0f, 0.0f)			//������Ϸ���
{
	printf("fuck you����ʼ������ɹ�!");
}

Camera::~Camera()
{

}


QVector3D Camera::getCameraPos() const { return cameraPos; }
QVector3D Camera::getCharacterPos() const { return characterPos; }
QVector3D Camera::getCameraFront() const { return cameraFront; }
QVector3D Camera::getCameraUp() const { return cameraUp; }
void Camera::resetVel() { velocity = QVector3D(0,0,0); }

QMatrix4x4 Camera::getVewMat()
{
	QMatrix4x4 viewMat;
	if (firstPerspect)
		viewMat.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	else
		viewMat.lookAt(QVector3D(characterPos.x(), characterPos.y() + 0.28f, characterPos.z() + 1.0f),
			characterPos, QVector3D(0.0f, 1.0f, 0.0f));
	return viewMat;
}

void Camera::turnPerspect()
{
	firstPerspect = !firstPerspect;
}

void Camera::updateCameraVectors(float yaw, float pitch)
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



void Camera::moveForward(const Collision& c)
{
	QVector3D movement = QVector3D(cameraFront.x(), 0, cameraFront.z()) * cameraSpeed;
	QVector3D temp = cameraPos;
	temp += movement;
	if (!c.allcollision(temp)) {
		cameraPos = temp;
		characterPos += movement;
	}


}

void Camera::moveBack(const Collision& c)
{
	QVector3D movement = QVector3D(cameraFront.x(), 0, cameraFront.z()) * cameraSpeed;
	QVector3D temp = cameraPos;
	temp -= movement;
	if (!c.allcollision(temp))
	{
		cameraPos = temp;
		characterPos -= movement;
	}

}

void Camera::moveLeft(const Collision& c)
{
	QVector3D movement = QVector3D::crossProduct(cameraFront, cameraUp) * cameraSpeed;
	QVector3D temp = cameraPos;
	temp -= movement;
	if (!c.allcollision(temp))
	{
		cameraPos = temp;
		characterPos -= movement;
	}

}

void Camera::moveRight(const Collision& c)
{
	QVector3D movement = QVector3D::crossProduct(cameraFront, cameraUp) * cameraSpeed;
	QVector3D temp = cameraPos;
	temp += movement;
	if (!c.allcollision(temp))
	{
		cameraPos = temp;
		characterPos += movement;
	}

}

void Camera::moveDown(const Collision& c)
{
	QVector3D movement = QVector3D(0.0f, -0.08f, 0.0f);
	QVector3D temp = cameraPos;
	temp += movement;
	if (!c.allcollision(temp))
	{
		cameraPos = temp;
		characterPos += movement;
	}

}

void Camera::moveUp(const Collision& c)
{
	QVector3D movement = QVector3D(0.0f, 0.25f, 0.0f);
	QVector3D temp = cameraPos;
	temp += movement;
	if (!c.allcollision(temp))
	{
		cameraPos = temp;
		characterPos += movement;
	}
}

void Camera::gravity(const Collision& c)
{
	velocity += QVector3D(0.0f, -0.05, 0.0f) * 0.1;
	QVector3D movement = velocity * 0.1;
	QVector3D temp = cameraPos;
	temp += movement;
	if (!c.allcollision(temp))
	{
		cameraPos = temp;
		characterPos += movement;
	}
}

#include<Camera.h>

Camera::Camera():
	firstPerspect(true),
	cameraSpeed(0.05f)
{

}

Camera::~Camera()
{

}

QVector3D Camera::getCameraPos() const { return cameraPos; }
QVector3D Camera::getCharacterPos() const{ return characterPos; }
QVector3D Camera::getCameraFront() const { return cameraFront; }


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

void Camera::setCamera()
{
	if (firstPerspect)
		gluLookAt(cameraPos.x(), cameraPos.y(), cameraPos.z(),
			cameraPos.x() + cameraFront.x(), cameraPos.y() + cameraFront.y(), cameraPos.z() + cameraFront.z(),
			cameraUp.x(), cameraUp.y(), cameraUp.z());
	else
		gluLookAt(characterPos.x(), characterPos.y() + 0.28f, characterPos.z() + 1.0f,
			characterPos.x(), characterPos.y(), characterPos.z(),
			0.0f, 1.0f, 0.0f);
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
	if (!c.allcollision(temp)) {
		cameraPos = temp;
		characterPos -= movement;
	}
}

void Camera::moveLeft(const Collision& c)
{
	QVector3D movement = QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
	QVector3D temp = cameraPos;
	temp -= movement;
	if (!c.allcollision(temp)) {
		cameraPos = temp;
		characterPos -= movement;
	}
}

void Camera::moveRight(const Collision& c)
{
	QVector3D movement = QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
	QVector3D temp = cameraPos;
	temp += movement;
	if (!c.allcollision(temp)) {
		cameraPos = temp;
		characterPos += movement;
	}
}

void Camera::moveDown(const Collision& c)
{
	QVector3D movement = QVector3D(0.0f, -0.08f, 0.0f);
	QVector3D temp = cameraPos;
	temp += movement;
	if (!c.allcollision(temp)) {
		cameraPos = temp;
		characterPos += movement;
	}
}


#include "../Headers/Camera.hpp"

Camera::Camera(int width, int height)
{
	this->position =	  { 0, 0, 0 };
	this->rightVector =   { 1, 0, 0 };
	this->upVector =	  { 0, 1, 0 };
	this->forwardVector = { 0, 0, 1 };
	this->speed = 1.f;
	this->runSpeed = 2.f;

	this->viewMatrix = XMMatrixLookAtLH(this->position, this->position + this->forwardVector, this->upVector);
	this->projectionMatrix = XMMatrixPerspectiveLH(3.141592f * 0.45f, (float)width / (float)height, 0.1f, 100.f);
	this->VPMatrix = this->viewMatrix * this->projectionMatrix;
}

Camera::~Camera()
{

}

void Camera::moveCamera(XMVECTOR translation, bool run)
{
	this->position += translation * (run ? this->runSpeed : this->speed);
	this->setViewMatrix(XMMatrixLookAtLH(this->position, this->position + this->forwardVector, this->upVector));
}

void Camera::setViewMatrix(XMMATRIX& matrix)
{
	this->viewMatrix = matrix;
	this->VPMatrix = this->viewMatrix * this->projectionMatrix;
}

void Camera::setProjectionMatrix(XMMATRIX& matrix)
{
	this->projectionMatrix = matrix;
	this->VPMatrix = this->viewMatrix * this->projectionMatrix;
}

XMMATRIX & Camera::getViewMatrix()
{
	return this->viewMatrix;
}

XMMATRIX & Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

XMMATRIX & Camera::getVPMatrix()
{
	return this->VPMatrix;
}

XMVECTOR & Camera::getForward()
{
	return this->forwardVector;
}

XMVECTOR & Camera::getPosition()
{
	return this->position;
}

XMVECTOR & Camera::getRight()
{
	return this->rightVector;
}

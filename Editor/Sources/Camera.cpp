#include "../Headers/Camera.hpp"
#include <iostream>
Camera::Camera(float width, float height)
{
	std::cout << (float)width / (float)height;
	this->position =	  { 0.f, 0.f, 0.f };
	this->rightVector =   { 1.f, 0.f, 0.f };
	this->upVector =	  { 0.f, 1.f, 0.f };
	this->forwardVector = { 0.f, 0.f, 1.f };
	this->speed = 1.f;
	this->runSpeed = 2.f;

	this->viewMatrix = XMMatrixLookAtLH(this->position, this->forwardVector + this->position, this->upVector);
	this->projectionMatrix = XMMatrixPerspectiveFovLH(3.141592f * 0.5f, width / height, 0.1f, 100.f);
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

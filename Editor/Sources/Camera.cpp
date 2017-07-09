#include "../Headers/Camera.hpp"

Camera::Camera(int width, int height)
{
	this->position =	  { 0, 0, 0 };
	this->rightVector =   { 1, 0, 0 };
	this->upVector =	  { 0, 1, 0 };
	this->forwardVector = { 0, 0, 1 };

	this->viewMatrix = XMMatrixLookToLH(this->position, this->forwardVector, this->upVector);
	this->projectionMatrix = XMMatrixPerspectiveFovLH(3.14159265359f / 2.f, (float)width / (float)height, 0.1f, 100.f);
	this->VPMatrix = this->projectionMatrix * this->viewMatrix;
}

Camera::~Camera()
{

}

void Camera::setViewMatrix(XMMATRIX& matrix)
{
	this->viewMatrix = matrix;
	this->VPMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setProjectionMatrix(XMMATRIX& matrix)
{
	this->projectionMatrix = matrix;
	this->VPMatrix = this->projectionMatrix * this->viewMatrix;
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

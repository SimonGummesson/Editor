#include "../Headers/Camera.hpp"
#include <iostream>
Camera::Camera(float width, float height, float rotationSpeed, float walkSpeed, float runSpeed)
{
	this->rotationSpeed = rotationSpeed;
	this->speed = walkSpeed;
	this->runSpeed = runSpeed;

	this->position = Vector3(0.f, 0.f, 0.f);
	this->rightVector = Vector3(1.f, 0.f, 0.f);
	this->upVector = Vector3(0.f, 1.f, 0.f);
	this->forwardVector = Vector3(0.f, 0.f, 1.f);

	this->viewMatrix = DirectX::XMMatrixLookAtLH(this->position, this->forwardVector + this->position, this->upVector);
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(3.141592f * 0.5f, width / height, 0.1f, 1000.f);
	this->VPMatrix = this->viewMatrix * this->projectionMatrix;
}

Camera::~Camera()
{

}

void Camera::moveCamera(Vector3 translation, bool run)
{
	this->position += translation * (run ? this->runSpeed : this->speed);
}

void Camera::setViewMatrix(Matrix matrix)
{
	this->viewMatrix = matrix;
	this->VPMatrix = this->viewMatrix * this->projectionMatrix;
}

void Camera::setProjectionMatrix(Matrix matrix)
{
	this->projectionMatrix = matrix;
	this->VPMatrix = this->viewMatrix * this->projectionMatrix;
}

void Camera::setRight(Vector3 vector)
{
	this->rightVector = vector;
}

void Camera::setForward(Vector3 vector)
{
	this->forwardVector = vector;
}

void Camera::setUp(Vector3 vector)
{
	this->upVector = vector;
}

Matrix Camera::getViewMatrix()
{
	return this->viewMatrix;
}

Matrix Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

Matrix Camera::getVPMatrix()
{
	return this->VPMatrix;
}

Vector3 Camera::getForward()
{
	return this->forwardVector;
}

Vector3 Camera::getPosition()
{
	return this->position;
}

Vector3 Camera::getRight()
{
	return this->rightVector;
}

Vector3 Camera::getUp()
{
	return this->upVector;
}

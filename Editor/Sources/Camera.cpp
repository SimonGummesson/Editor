#include "../Headers/Camera.hpp"
#include <iostream>
Camera::Camera(float width, float height, float rotationSpeed, float walkSpeed, float runSpeed, float cameraHeight)
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

	this->heightMap = nullptr;

	this->cameraHeight = cameraHeight;
}

Camera::~Camera()
{

}

void Camera::update(InputStatus& inputs, float dt)
{
	bool running = false;

	if (inputs.ShiftKey)
		running = true;
	if (inputs.WKey)
		moveCamera(Vector3(forwardVector.x, 0.f, forwardVector.z) * dt, running);
	if (inputs.AKey)
		moveCamera(-getRight() * dt, running);
	if (inputs.SKey)
		moveCamera(-Vector3(forwardVector.x, 0.f, forwardVector.z) * dt, running);
	if (inputs.DKey)
		moveCamera(getRight() * dt, running);
	if (inputs.SpaceKey)
		moveCamera({ 0, dt, 0 }, running);
	if (inputs.LCTRLKey)
		moveCamera({ 0, -dt, 0 }, running);

	if (inputs.LMouse)
	{
		ShowCursor(FALSE);
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);

		//Horizontal camera rotation
		float deltaX = (float)currentMousePos.x - (float)lastCursorPosition.x;
		if (deltaX != 0.f)
		{
			float sign = signbit(deltaX) ? -1.0f : 1.0f;
			Matrix rot = DirectX::XMMatrixRotationY(sign * rotationSpeed * dt);
			setRight(Vector3::Transform(getRight(), rot));
			setForward(Vector3::Transform(getForward(), rot));
			setUp(Vector3::Transform(getUp(), rot));
		}

		//Vertical camera rotation
		float deltaY = (float)currentMousePos.y - (float)this->lastCursorPosition.y;
		if (deltaY != 0.f)
		{
			float sign = signbit(deltaY) ? -1.0f : 1.0f;
			Matrix rot = DirectX::XMMatrixRotationAxis(getRight(), sign * rotationSpeed * dt);
			setUp(Vector3::Transform(getUp(), rot));
			setForward(Vector3::Transform(getForward(), rot));
			setRight(Vector3::Transform(getRight(), rot));
		}
		this->lastCursorPosition = currentMousePos;
	}
	else
		ShowCursor(TRUE);

	float distanceToGround = heightMap->getIntersection(position);
	if (distanceToGround < cameraHeight && distanceToGround != -1.f)
		translateCamera((cameraHeight - distanceToGround) * Vector3(0.f, 1.f, 0.f));
	if (distanceToGround > cameraHeight)
		translateCamera(Vector3(0.f, -1.f, 0.f) * 9.82f * dt);

	setViewMatrix(DirectX::XMMatrixLookAtLH(getPosition(), getPosition() + getForward(), getUp()));
}

void Camera::moveCamera(Vector3 translation, bool run)
{
	this->position += translation * (run ? this->runSpeed : this->speed);
}

void Camera::translateCamera(Vector3 translation)
{
	this->position += translation;
}

void Camera::setPosition(Vector3 vector)
{
	position = vector;
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

void Camera::setHeightMap(HeightMap * heightMap)
{
	this->heightMap = heightMap;
}

void Camera::setHeight(float height)
{
	cameraHeight = height;
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

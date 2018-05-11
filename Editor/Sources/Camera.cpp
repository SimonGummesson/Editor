#include "../Headers/Camera.hpp"
#include <iostream>
Camera::Camera(float width, float height, float rotationSpeed, float walkSpeed, float runSpeed, float cameraHeight)
{
	this->rotationSpeed = rotationSpeed;
	speed = walkSpeed;
	this->runSpeed = runSpeed;

	position = Vector3(0.f, 0.f, 7.5f);
	rightVector = Vector3(1.f, 0.f, 0.f);
	upVector = Vector3(0.f, 1.f, 0.f);
	forwardVector = Vector3(0.f, 0.f, 1.f);

	viewMatrix = DirectX::XMMatrixLookAtLH(position, forwardVector + position, upVector);
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(3.141592f * 0.5f, width / height, 0.1f, 1000.f);
	VPMatrix = viewMatrix * projectionMatrix;

	heightMap = nullptr;

	this->cameraHeight = cameraHeight;
	mousePressed = false;
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
		if (mousePressed == true)
		{
			ShowCursor(FALSE);
			POINT currentMousePos;
			GetCursorPos(&currentMousePos);

			//Horizontal camera rotation
			Matrix rot = DirectX::XMMatrixRotationY(((float)currentMousePos.x - 400.f) * rotationSpeed * dt);
			setRight(Vector3::Transform(getRight(), rot));
			setForward(Vector3::Transform(getForward(), rot));
			setUp(Vector3::Transform(getUp(), rot));

			//Vertical camera rotation
			rot = DirectX::XMMatrixRotationAxis(getRight(), ((float)currentMousePos.y - 400.f) * rotationSpeed * dt);

			if (Vector3(0.f, 1.f, 0.f).Dot(Vector3::Transform(getForward(), rot)) > 0.9f)
			{
				forwardVector = Vector3(0.f, 0.f, 1.f);
				upVector = Vector3(0.f, 1.f, 0.f);
				rot = DirectX::XMMatrixRotationAxis(getRight(), 3.14159265f * 0.45f);
			}

			setForward(Vector3::Transform(getForward(), rot));
			setUp(Vector3::Transform(getUp(), rot));
			setRight(Vector3::Transform(getRight(), rot));

			SetCursorPos(400, 400);
		}
		else
		{
			ShowCursor(FALSE);
			SetCursorPos(400, 400);
			mousePressed = true;
		}
	}
	else
	{
		mousePressed = false;
		ShowCursor(TRUE);
	}

	float distanceToGround = heightMap->getIntersection(position);
	if (distanceToGround < cameraHeight && distanceToGround != -1.f)
		translateCamera((cameraHeight - distanceToGround) * Vector3(0.f, 1.f, 0.f));
	if (distanceToGround > cameraHeight)
		translateCamera(Vector3(0.f, -1.f, 0.f) * 9.82f * dt);

	setViewMatrix(DirectX::XMMatrixLookAtLH(getPosition(), getPosition() + getForward(), getUp()));
}

void Camera::moveCamera(Vector3 translation, bool run)
{
	this->position += translation * (run ? runSpeed : speed);
}

void Camera::translateCamera(Vector3 translation)
{
	position += translation;
}

void Camera::setPosition(Vector3 vector)
{
	position = vector;
}

void Camera::setViewMatrix(Matrix matrix)
{
	viewMatrix = matrix;
	VPMatrix = viewMatrix * projectionMatrix;
}

void Camera::setProjectionMatrix(Matrix matrix)
{
	projectionMatrix = matrix;
	VPMatrix = viewMatrix * projectionMatrix;
}

void Camera::setRight(Vector3 vector)
{
	rightVector = vector;
}

void Camera::setForward(Vector3 vector)
{
	forwardVector = vector;
}

void Camera::setUp(Vector3 vector)
{
	upVector = vector;
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
	return viewMatrix;
}

Matrix Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

Matrix Camera::getVPMatrix()
{
	return VPMatrix;
}

Vector3 Camera::getForward()
{
	return forwardVector;
}

Vector3 Camera::getPosition()
{
	return position;
}

Vector3 Camera::getRight()
{
	return rightVector;
}

Vector3 Camera::getUp()
{
	return upVector;
}

Vector3 * Camera::getPositionPointer()
{
	return &position;
}

Matrix * Camera::getWPMatrixPointer()
{
	return &VPMatrix;
}

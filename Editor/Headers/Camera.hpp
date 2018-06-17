#pragma once
#include "SimpleMath.h"
#include "structs.hpp"
#include "HeightMap.hpp"
using namespace DirectX::SimpleMath;

class Camera
{
private:
	Vector3 position;
	Vector3 forwardVector;
	Vector3 upVector;
	Vector3 rightVector;

	Matrix viewMatrix;
	Matrix projectionMatrix;
	Matrix VPMatrix;

	POINT lastCursorPosition;

	HeightMap* heightMap;

	float cameraHeight;
	bool mousePressed;
public:
	Camera(float width, float height, float rotationSpeed, float walkSpeed, float runSpeed, float cameraHeight);
	~Camera();
	void update(InputStatus& inputs, float dt);
	float rotationSpeed;
	float speed;
	float runSpeed;
	void moveCamera(Vector3 translation, bool run);
	void translateCamera(Vector3 translation);
	void setPosition(Vector3 vector);
	void setViewMatrix(Matrix matrix);
	void setProjectionMatrix(Matrix matrix);
	void setRight(Vector3 vector);
	void setForward(Vector3 vector);
	void setUp(Vector3 vector);
	void setHeightMap(HeightMap* heightMap);
	void setHeight(float height);
	Matrix getViewMatrix() const;
	Matrix getProjectionMatrix() const;
	Matrix getVPMatrix() const;
	Vector3 getForward() const;
	Vector3 getPosition() const;
	Vector3 getRight() const;
	Vector3 getUp() const;
	Vector3* const getPositionPointer();
	Matrix* const getWPMatrixPointer();
};
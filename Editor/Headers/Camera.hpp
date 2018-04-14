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
	Matrix getViewMatrix();
	Matrix getProjectionMatrix();
	Matrix getVPMatrix();
	Vector3 getForward();
	Vector3 getPosition();
	Vector3 getRight();
	Vector3 getUp();
	Vector3* getPositionPointer();
	Matrix* getWPMatrixPointer();
};
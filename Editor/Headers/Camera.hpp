#pragma once
#include "SimpleMath.h"
#include "SimpleMath.inl"

using namespace DirectX;

__declspec(align(16)) class Camera
{
private:
	XMVECTOR position;
	XMVECTOR forwardVector;
	XMVECTOR upVector;
	XMVECTOR rightVector;

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX VPMatrix;
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	Camera(float width, float height, float rotationSpeed, float walkSpeed, float runSpeed);
	~Camera();
	float rotationSpeed;
	float speed;
	float runSpeed;
	void moveCamera(XMVECTOR translation, bool run);
	void setViewMatrix(XMMATRIX& matrix);
	void setProjectionMatrix(XMMATRIX& matrix);
	void setRight(XMVECTOR& vector);
	void setForward(XMVECTOR& vector);
	void setUp(XMVECTOR& vector);
	XMMATRIX &getViewMatrix();
	XMMATRIX &getProjectionMatrix();
	XMMATRIX &getVPMatrix();
	XMVECTOR &getForward();
	XMVECTOR &getPosition();
	XMVECTOR &getRight();
	XMVECTOR &getUp();
};
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

	float speed;
	float runSpeed;
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
	Camera(int width, int height);
	~Camera();
	void moveCamera(XMVECTOR translation, bool run);
	void setViewMatrix(XMMATRIX& matrix);
	void setProjectionMatrix(XMMATRIX& matrix);
	XMMATRIX &getViewMatrix();
	XMMATRIX &getProjectionMatrix();
	XMMATRIX &getVPMatrix();
	XMVECTOR &getForward();
	XMVECTOR &getPosition();
	XMVECTOR &getRight();
};
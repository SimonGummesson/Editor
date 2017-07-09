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
	Camera(int width, int height);
	~Camera();
	void setViewMatrix(XMMATRIX& matrix);
	void setProjectionMatrix(XMMATRIX& matrix);
	XMMATRIX &getViewMatrix();
	XMMATRIX &getProjectionMatrix();
	XMMATRIX &getVPMatrix();
};
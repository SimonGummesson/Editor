#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "SimpleMath.h"
#include "SimpleMath.inl"
#include "structs.hpp"

using namespace DirectX;

__declspec(align(16)) class Object
{
protected:
	std::string name;
	XMMATRIX worldMatrix;
	XMVECTOR translation;
	XMVECTOR rotation;
	XMVECTOR scaling;

public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	Object(std::string name);
	virtual ~Object();
	XMMATRIX &getWorldMatrix();
	void translate(XMVECTOR translation);
	void setTranslation(XMVECTOR translation);
	void rotate(XMVECTOR rotation);
	void setRotation(XMVECTOR rotation);
	void scale(XMVECTOR scale);
	void setScale(XMVECTOR scale);
	void updateWorldMatrix();
	virtual void update(float dt) = 0;
	std::string getName();
};
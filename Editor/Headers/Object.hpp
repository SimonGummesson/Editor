#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "SimpleMath.h"
#include "SimpleMath.inl"
#include "structs.hpp"

__declspec(align(16)) class Object
{
private:
	std::string name;
	DirectX::XMMATRIX worldMatrix;
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
	~Object();
	DirectX::XMMATRIX &getWorldMatrix();
	void translate(DirectX::XMVECTOR translation);
	void scale(DirectX::XMVECTOR translation);
	void update();
	std::string getName();
};
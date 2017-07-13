#pragma once
#include <vector>
#include <iostream>

#include "SimpleMath.h"
#include "SimpleMath.inl"
#include "structs.hpp"

__declspec(align(16)) class Object
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int indexCount;
	unsigned int vertexCount;
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
	Object();
	~Object();
	DirectX::XMMATRIX &getWorldMatrix();
	void translate(DirectX::XMVECTOR translation);
	void scale(DirectX::XMVECTOR translation);
	void update();
	void setBuffers(ID3D11Device * device, std::vector<Vertex> vertexes, std::vector<unsigned int> indices);
	ID3D11Buffer** getVertexBuffer();
	void draw(ID3D11DeviceContext * deviceContext);
};
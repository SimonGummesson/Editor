#pragma once
#include <vector>
#include <iostream>

#include "SimpleMath.h"
#include "SimpleMath.inl"
#include "structs.hpp"

using namespace DirectX;
class Object
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT32 vertexSize;
	UINT32 offset;
	unsigned int indexCount;
public:
	Object();
	~Object();
	void setBuffers(ID3D11Device* device, std::vector<Vertex> vertexes, UINT32 offset, UINT32 vertexSize, std::vector<unsigned int> indices);
	void draw(ID3D11DeviceContext &deviceContext);
};
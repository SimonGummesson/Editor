#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "SimpleMath.h"
#include "SimpleMath.inl"
#include "structs.hpp"
#include <d3d11.h>
class ObjectData
{
public:
	ObjectData(std::string name, ID3D11Device* device, std::vector<Vertex> vertexes, std::vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
	std::string getName();
	ID3D11Buffer** getVertexBuffer();
	ID3D11Buffer** getIndexBuffer();
	unsigned int getVertexCount();
	unsigned int getIndexCount();
	D3D_PRIMITIVE_TOPOLOGY getPrimitiveTopology();
	~ObjectData();
private:
	std::string name;
	ID3D11Buffer* vertexBuffer;
	unsigned int vertexCount;
	ID3D11Buffer* indexBuffer;
	unsigned int indexCount;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;
};
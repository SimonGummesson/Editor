#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "SimpleMath.h"
#include "SimpleMath.inl"
#include "structs.hpp"
#include "Object.hpp"
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
	void addOject(Object* object);
	void Draw(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX& VPMatrix, ID3D11Buffer* cbuffer);
	void updateBuffer(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX &worldMatrix, DirectX::XMMATRIX & VPMatrix, ID3D11Buffer* cbuffer);
	~ObjectData();
private:
	std::string name;
	ID3D11Buffer* vertexBuffer;
	unsigned int vertexCount;
	ID3D11Buffer* indexBuffer;
	unsigned int indexCount;
	std::vector<Object*> objects;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;
};
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SimpleMath.h"
#include "structs.hpp"
#include "Object.hpp"
#include <d3d11.h>
#include "WICTextureLoader.h"

using namespace std;
using namespace DirectX::SimpleMath;

class ObjectData
{
public:
	ObjectData(string name, ID3D11Device* device, vector<VertexColor> vertexes, vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
	ObjectData(string name, ID3D11Device* device, vector<VertexUV> vertexes, vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
	ObjectData(string name, string fileName, ID3D11Device* device);
	materialLightData lightData;
	string getName();
	ID3D11Buffer** getVertexBuffer();
	ID3D11Buffer** getIndexBuffer();
	ID3D11ShaderResourceView** getTextureView();

	unsigned int getVertexCount();
	unsigned int getIndexCount();
	D3D_PRIMITIVE_TOPOLOGY getPrimitiveTopology();
	void addOject(Object* object);
	void Draw(ID3D11DeviceContext *deviceContext, Matrix VPMatrix, ID3D11Buffer* cbuffer);
	void updateBuffer(ID3D11DeviceContext *deviceContext, Matrix worldMatrix, Matrix VPMatrix, ID3D11Buffer* cbuffer);
	bool getType();
	~ObjectData();
private:
	bool isTexture;
	string ExtractNumber(char &check, ifstream &objFile);
	string name;
	ID3D11Buffer* vertexBuffer;
	unsigned int vertexCount;
	ID3D11Buffer* indexBuffer;
	unsigned int indexCount;
	vector<Object*> objects;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;
	ID3D11Resource* texture = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;
};
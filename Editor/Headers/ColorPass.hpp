#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <iostream>

#include "Object.hpp"
#include "ObjectData.hpp"
#include "structs.hpp"
using namespace std;

class ColorPass
{
public:
	void drawPass(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX& VPMatrix);
	void update();
	void setVertexShaderAndLayout(ID3D11Device *device, LPCWSTR path);
	void setVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset);
	void setPixelShader(ID3D11Device *device, LPCWSTR path);
	void addObject(Object *object);
	void addObjectData(ObjectData *objectdata);
	void updateBuffer(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX &worldMatrix);
	ColorPass(ID3D11Device * device);
	~ColorPass();
private:
	vector<Object*> objects;
	vector<ObjectData*> objectData;
	ID3D11InputLayout* vertexLayout;
	ID3D11VertexShader *vertexShader;
	UINT32 vertexSize;
	UINT32 offset;
	ID3D11HullShader *hullShader;
	ID3D11DomainShader *domainShader;
	ID3D11GeometryShader *geometryShader;
	ID3D11PixelShader *pixelShader;
	ID3D11Buffer* VSConstantBuffer;
};
#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>

class Pass
{
protected:
	ID3D11DeviceContext* deviceContext;

	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* vertexLayout;
	UINT32 vertexSize;
	UINT32 vertexOffset;

	//ID3D11HullShader* hullShader;
	
	//ID3D11DomainShader* domainShader;

	ID3D11GeometryShader* geometryShader;
	
	ID3D11PixelShader* pixelShader;
public:
	Pass();
	virtual ~Pass();
	void setVertexShaderAndLayout(ID3D11Device * device, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numInputElements, LPCWSTR path);
	void setVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset);
	void setGeometryShader(ID3D11Device * device, LPCWSTR path);
	void setPixelShader(ID3D11Device * device, LPCWSTR path);

	void setDeviceContext(ID3D11DeviceContext* deviceContext);
	virtual void drawPass() = 0;
	virtual void update(float dt) = 0;
	//TODO when needed
	//void setHullShader()
	//void setTesselator()
	//void setDomainShader()
};
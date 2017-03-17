#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <iostream>

#include "Object.hpp"
using namespace std;

class ColorPass
{
public:
	void drawPass(ID3D11DeviceContext &deviceContext);
	void setVertexShaderAndLayout(ID3D11Device &device, LPCWSTR path);
	void setPixelShader(ID3D11Device &device, LPCWSTR path);
	void addObject(Object *object);
	ColorPass();
	~ColorPass();
private:
	vector<Object*> objects;
	ID3D11InputLayout* vertexLayout;
	ID3D11VertexShader *vertexShader;
	ID3D11HullShader *hullShader;
	ID3D11DomainShader *domainShader;
	ID3D11GeometryShader *geometryShader;
	ID3D11PixelShader *pixelShader;
};
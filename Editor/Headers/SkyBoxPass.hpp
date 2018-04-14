#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "SimpleMath.h"

using namespace std;
using namespace DirectX::SimpleMath;

class SkyBoxPass
{
public:
	void drawPass(ID3D11DeviceContext *deviceContext, Matrix VPMatrix, Vector3 cameraPos);
	

	SkyBoxPass(ID3D11Device * device);
	~SkyBoxPass();
private:
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
};
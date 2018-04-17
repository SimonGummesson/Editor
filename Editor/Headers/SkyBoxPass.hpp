#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "SimpleMath.h"
#include <string>

#include "Pass.hpp"
#include "../Headers/stb_image.h"
#include "SkyBoxObject.hpp"
#include "ObjectData.hpp"
#include "structs.hpp"

using namespace std;
using namespace DirectX::SimpleMath;

class SkyBoxPass : public Pass
{
public:
	virtual void drawPass();
	virtual void update(float dt);

	void setCubeMap(string path, string fileExtension);
	SkyBoxPass(ID3D11Device * device, ID3D11DeviceContext* context, float dist, Matrix* WPMatrix, Vector3* cameraPos);
	~SkyBoxPass();
private:
	ID3D11ShaderResourceView* cubeMapSrv;
	ID3D11Device* device;
	SkyBoxObject* cubeObject;
	ObjectData* skyBoxObjectData;
	ID3D11Buffer* WVPConstantBuffer;
	ID3D11RasterizerState* rasterizerState;
	ID3D11DepthStencilState* depthStencilstate;
	ID3D11SamplerState* skyMapSamplerState;

	Matrix* WPMatrix;
};
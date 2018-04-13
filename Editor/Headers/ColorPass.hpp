#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <iostream>
#include <map>

#include "Object.hpp"
#include "ObjectData.hpp"
#include "structs.hpp"
#include "UniversalStructs.hpp"

using namespace std;
using namespace DirectX::SimpleMath;

class ColorPass
{
public:
	void drawPass(ID3D11DeviceContext *deviceContext, Matrix VPMatrix, Vector3 cameraPos);
	void update(float dt);
	void setColorVertexShaderAndLayout(ID3D11Device *device, LPCWSTR path);
	void setColorVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset);
	void setColorPixelShader(ID3D11Device *device, LPCWSTR path);
	void setColorGeometryShader(ID3D11Device *device, LPCWSTR path);

	void setTextureVertexShaderAndLayout(ID3D11Device *device, LPCWSTR path);
	void setTextureVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset);
	void setTexturePixelShader(ID3D11Device *device, LPCWSTR path);

	bool addObject(Object *object);
	void addObjectData(ObjectData *objectdata);
	void updatePSBuffer(ID3D11DeviceContext *deviceContext, Vector3 cameraPos);
	void updatePSLightBuffer(ID3D11DeviceContext *deviceContext, materialLightData data);

	void setAmbientLight(Vector3 ambient);
	void addLight(Light light, string name);
	void removeLight(Light light);
	void updateLightBuffer(ID3D11DeviceContext * deviceContext);
	ColorPass(ID3D11Device * device);
	~ColorPass();
private:
	// Variables for objects using color
	vector<Object*> objects;
	vector<ObjectData*> objectDataColor;
	ID3D11InputLayout* colorVertexLayout;
	ID3D11VertexShader *colorVertexShader;
	UINT32 colorVertexSize;
	UINT32 colorOffset;
	ID3D11GeometryShader *colorGeometryShader;
	ID3D11PixelShader *colorPixelShader;

	ID3D11Buffer* GSConstantBuffer;
	ID3D11Buffer* PSConstantBuffer;
	ID3D11Buffer* PSLightDataConstantBuffer;

	// Variables for objects using texture
	vector<ObjectData*> objectDataTexture;
	ID3D11InputLayout* textureVertexLayout;
	ID3D11VertexShader *textureVertexShader;
	UINT32 textureVertexSize;
	UINT32 textureOffset;
	ID3D11PixelShader *texturePixelShader;

	//light members
	ID3D11Buffer* lightBuffer;
	ID3D11ShaderResourceView* lightSRV;
	vector<Light> lights;
	Vector3 ambientLight;
};
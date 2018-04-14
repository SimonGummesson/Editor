#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <iostream>
#include <map>

#include "Pass.hpp"
#include "Object.hpp"
#include "ObjectData.hpp"
#include "structs.hpp"
#include "UniversalStructs.hpp"

using namespace std;
using namespace DirectX::SimpleMath;

class TexturePass : public Pass
{
public:
	virtual void drawPass();
	virtual void update(float dt);

	bool addObject(Object *object);
	void addObjectData(ObjectData *objectdata);
	void updatePSBuffer(ID3D11DeviceContext *deviceContext, Vector3 cameraPos);
	void updatePSLightBuffer(ID3D11DeviceContext *deviceContext, materialLightData data);

	void setAmbientLight(Vector3 ambient);
	void addLight(Light light, string name);
	void removeLight(Light light);
	void updateLightBuffer(ID3D11DeviceContext * deviceContext);
	TexturePass(ID3D11Device * device, ID3D11DeviceContext* deviceContext, Matrix* cameraVPMatrix, Vector3* cameraPos);
	~TexturePass();
private:
	Matrix* cameraVPMatrix;
	Vector3* cameraPos;

	vector<Object*> objects;
	vector<ObjectData*> objectData;

	ID3D11Buffer* GSConstantBuffer;
	ID3D11Buffer* PSConstantBuffer;
	ID3D11Buffer* PSLightDataConstantBuffer;

	//light members
	ID3D11Buffer* lightBuffer;
	ID3D11ShaderResourceView* lightSRV;
	vector<Light> lights;
	Vector3 ambientLight;
};
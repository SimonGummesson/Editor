#pragma once
#include "SimpleMath.h"
#include "SimpleMath.inl"
struct VertexColor {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	VertexColor(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color) : position(pos), color(color)
	{

	}
};

struct VertexUV {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 normal;
	VertexUV(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 UV) : position(pos), UV(UV)
	{

	}
	VertexUV() { }
};

struct GS_COLORPASS_CONSTANT_BUFFER
{
	DirectX::XMMATRIX WVPMatrix;
	DirectX::XMMATRIX WorldMatrix;
};

struct PS_COLORPASS_CONSTANT_BUFFER
{
	DirectX::XMFLOAT3 eyePos;
	float roughnessValue ;
	DirectX::XMFLOAT3 lightPosition;
	float F0; // fresnel reflectance at normal incidence
	DirectX::XMFLOAT3 eyeForward;
	float k; // fraction of diffuse reflection (specular reflection = 1 - k)
};

struct materialLightData
{
	DirectX::XMFLOAT3 ambientColor;
	float pad;
	DirectX::XMFLOAT3 diffuseColor;
	float pad1;
	DirectX::XMFLOAT3 specularColor;
	float specularPower;
	materialLightData() 
	{
		ambientColor = { 1.f, 1.f, 1.f };
		diffuseColor = { 1.f, 1.f, 1.f };
		specularColor = { 1.f, 1.f, 1.f };
		specularPower = 1.f;
	}
};

#pragma once
#include "SimpleMath.h"
#include "SimpleMath.inl"
struct Vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color) : position(pos), color(color)
	{

	}
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
	float k; // fraction of diffuse reflection (specular reflection = 1 - k)
	DirectX::XMFLOAT3 pad;
};

#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

struct InputStatus {
	SHORT WKey, AKey, SKey, DKey;
	SHORT ShiftKey, SpaceKey, LCTRLKey;
	SHORT LMouse;
};

struct VertexColor {
	Vector3 position;
	Vector3 color;
	VertexColor(Vector3 pos, Vector3 color) : position(pos), color(color)
	{

	}
	VertexColor()
	{

	}
};

struct VertexUV {
	Vector3 position;
	Vector2 UV;
	Vector3 normal;
	VertexUV(Vector3 pos, Vector2 UV) : position(pos), UV(UV)
	{

	}
	VertexUV() { }
};

struct GS_COLORPASS_CONSTANT_BUFFER
{
	Matrix WVPMatrix;
	Matrix WorldMatrix;
};

struct materialLightData
{
	Vector3 ambientColor;
	float pad;
	Vector3 diffuseColor;
	float pad1;
	Vector3 specularColor;
	float specularPower;
	materialLightData()
	{
		ambientColor = Vector3(1.f, 1.f, 1.f);
		diffuseColor = Vector3(1.f, 1.f, 1.f);
		specularColor = Vector3(1.f, 1.f, 1.f);
		specularPower = 1.f;
	}
};

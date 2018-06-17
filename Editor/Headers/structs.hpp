#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

struct InputStatus {
	SHORT WKey, AKey, SKey, DKey;
	SHORT ShiftKey, SpaceKey, LCTRLKey;
	SHORT RMouse;
};

struct VertexColor {
	Vector3 position;
	Vector3 color;
	Vector3 normal;
	VertexColor(Vector3 pos, Vector3 color, Vector3 nor) : position(pos), color(color), normal(nor)
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
	VertexUV(float posX, float posY, float posZ, float UVu, float UVv, float normalX, float normalY, float normalZ)
	{
		position.x = posX;
		position.y = posY;
		position.z = posZ;

		UV.x = UVu;
		UV.y = UVv;

		normal.x = normalX;
		normal.y = normalY;
		normal.z = normalZ;
	}
	VertexUV() { }
};

struct WVP_BUFFER
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

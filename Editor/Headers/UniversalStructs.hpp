#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

struct byte4UInt
{
	UINT x, y, z, w;
};

struct float4
{
	float x, y, z, w;
	float4(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w;}
	float4() {};
	bool operator==(const float4& other)
	{
		if (x == other.x && y == other.y && z == other.z && w == other.w)
			return true;
		else
			return false;
	}
};

enum LightType { POINT_LIGHT, DIRECTIONAL_LIGHT };
// This header will be included in the pixel shaders
struct Light
{
	LightType type;
	float attA, attB, maxDist; // for point lights only
	float4 diffuseColor;
	float4 specularColor;
	union
	{
		float4 position;
		float4 direction;
	};

	Light(LightType type)
	{
		this->type = type;
		if (POINT_LIGHT == type)
			position = float4(0.f, 0.f, 0.f, 0.f);
		else if (DIRECTIONAL_LIGHT == type)
			direction = float4(0.f, 0.f, 0.f, 0.f);
	}
	Light& operator=(const Light& other)
	{
		// check for self-assignment
		if (&other == this)
			return *this;
		// reuse storage when possible
		type = other.type;
		attA = other.attA;
		attB = other.attB;
		maxDist = other.maxDist;

		diffuseColor = other.diffuseColor;
		specularColor = other.specularColor;
		
		if (POINT_LIGHT == type)
			position = other.position;
		else if (DIRECTIONAL_LIGHT == type)
			direction = other.direction;
		return *this;
	}

	bool operator==(const Light& other)
	{
		if (type == other.type && diffuseColor == other.diffuseColor && specularColor == other.specularColor && attA == other.attA && attB == other.attB && maxDist == other.maxDist)
		{
			if (POINT_LIGHT == type && position == other.position)
				return true;
			else if (DIRECTIONAL_LIGHT == type && direction == other.direction)
				return true;
			else
				return false;
		}
		else
			return false;
	}
};

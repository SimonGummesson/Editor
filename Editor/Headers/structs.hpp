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
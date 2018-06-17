#include "..\Headers\HeightMap.hpp"
#include <iostream>
HeightMap::HeightMap(std::string name, std::vector<VertexColor> vertices, Vector2 quadDimension) : Object(name)
{
	this->quadDimension = quadDimension;
	vertexData = vertices;
}

HeightMap::~HeightMap()
{
	
}

void HeightMap::update(float dt)
{
	return;
}

float HeightMap::getIntersection(Vector3 pos) const
{
	// Transfer position into heightmaps model space
	pos = pos - translation;
	Matrix rot = DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(-rotation));
	pos = DirectX::XMVector3TransformCoord(pos, rot);
	pos = pos / scaling;

	// Do calculation
	if (pos.x < 0.f || pos.x > width * quadWidth || pos.z < 0.f || pos.z > height * quadHeight)
		return -1.f;
	int xPos, zPos;
	xPos = (int)floor(pos.x / quadWidth);
	zPos = (int)floor(pos.z / quadHeight);

 	Vector3 v0 = vertexData[zPos * width + xPos].position;
	Vector3 v1 = vertexData[(zPos + 1) * width + xPos].position;
	Vector3 v2 = vertexData[zPos * width + xPos + 1].position;
	Vector3 v3 = vertexData[(zPos + 1) * width + xPos + 1].position;

	float t = triangleIntersection(v0, v1, v2, Vector3(0.f, -1.f, 0.f), pos);
	if (t > 0.f)
		return t;

	t = triangleIntersection(v3, v2, v1, Vector3(0.f, -1.f, 0.f), pos);
	if (t > 0.f)
		return t;
	return -1.f;
}

float HeightMap::triangleIntersection(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 dir, Vector3 origin) const
{
	bool hit = false;
	Vector3 edge0 = v1 - v0;
	Vector3 edge1 = v2 - v0;
	Vector3 q = dir.Cross(edge1);
	float a = edge0.Dot(q);
	if (!(a > -0.00001f && a < 0.00001f))
	{
		float f = 1.f / a;
		Vector3 s = origin - v0;
		float u = f*(s.Dot(q));
		if (u >= 0.f)
		{
			Vector3 r = s.Cross(edge0);
			float v = f*(dir.Dot(r));
			if (v >= 0.f && u + v <= 1.f)
			{
				float t = f*(edge1.Dot(r));
				if (t > 0.001f)
					return t;
			}
		}
	}
	return -1.f;
}

void HeightMap::setQuadDimensions(float quadWidth, float quadHeight)
{
	this->quadWidth = quadWidth;
	this->quadHeight = quadHeight;
}

void HeightMap::setDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}

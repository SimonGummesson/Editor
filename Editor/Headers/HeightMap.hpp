#pragma once
#include "Object.hpp"
#include <math.h>
class HeightMap : public Object
{
public:
	HeightMap(std::string name, std::vector<VertexColor> vertices, Vector2 quadDimension);
	~HeightMap();
	void update(float dt) override;
	float getIntersection(Vector3 pos);
	float triangleIntersection(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 dir, Vector3 origin);
	void setQuadDimensions(float quadWidth, float quadHeight);
	void setDimensions(int width, int height);
private:
	Vector2 quadDimension;
	float quadWidth, quadHeight;
	int width, height;
	std::vector<VertexColor> vertexData;
};


// v1.----------.v3
//   |          |
//   |          |
//   |          |
//   |          |
// v0.----------.v2
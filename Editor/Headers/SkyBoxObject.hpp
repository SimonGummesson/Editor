#pragma once
#include "Object.hpp"

class SkyBoxObject : public Object
{
public:
	SkyBoxObject(std::string name, Vector3* cameraPos);
	~SkyBoxObject();
	void update(float dt) override;
private:
	Vector3* cameraPos;
};
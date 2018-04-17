#pragma once
#include "..\Headers\SkyBoxObject.hpp"

SkyBoxObject::SkyBoxObject(std::string name, Vector3* cameraPos) : Object(name)
{
	this->cameraPos = cameraPos;
}

SkyBoxObject::~SkyBoxObject()
{

}

void SkyBoxObject::update(float dt)
{
	translation = *cameraPos;
	updateWorldMatrix();
}

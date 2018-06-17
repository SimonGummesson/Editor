#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "SimpleMath.h"
#include "structs.hpp"

using namespace DirectX::SimpleMath;

class Object
{
protected:
	std::string name;
	Matrix worldMatrix;
	Vector3 translation;
	Vector3 rotation;
	Vector3 scaling;

public:
	explicit Object(std::string name);
	virtual ~Object();
	Matrix getWorldMatrix() const;
	Vector3 getTranslation() const;
	Vector3* const getTranslationPointer();
	void translate(Vector3 translation);
	void setTranslation(Vector3 translation);
	void rotate(Vector3 rotation);
	void setRotation(Vector3 rotation);
	void scale(Vector3 scale);
	void setScale(Vector3 scale);
	void updateWorldMatrix();
	virtual void update(float dt) = 0;
	std::string getName() const;
};
#include "../Headers/Object.hpp"

Object::Object(std::string name)
{
	this->name = name;
	worldMatrix = DirectX::XMMatrixIdentity();
	translation = Vector3(0.f, 0.f, 0.f);
	rotation = Vector3(0.f, 0.f, 0.f);
	scaling = Vector3(1.f, 1.f, 1.f);
}

Object::~Object()
{
}

Matrix Object::getWorldMatrix()
{
	return worldMatrix;
}

Vector3 Object::getTranslation()
{
	return translation;
}

void Object::translate(Vector3 translation)
{
	this->translation += translation;
}

void Object::setTranslation(Vector3 translation)
{
	this->translation = translation;
}

void Object::rotate(Vector3 rotation)
{
	this->rotation += rotation;
}

void Object::setRotation(Vector3 rotation)
{
	this->rotation = rotation;
}

void Object::scale(Vector3 scale)
{
	scaling += scale;
}

void Object::setScale(Vector3 scale)
{
	scaling = scale;
}

void Object::updateWorldMatrix()
{
	worldMatrix = DirectX::XMMatrixScalingFromVector(scaling) * DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(rotation)) * XMMatrixTranslationFromVector(translation);
}

std::string Object::getName()
{
	return name;
}


#include "../Headers/Object.hpp"

Object::Object(std::string name)
{
	this->name = name;
	this->worldMatrix = DirectX::XMMatrixIdentity();
	this->translation = Vector3(0, 0, 0);
	this->rotation = Vector3(0, 0, 0);
	this->scaling = Vector3(1, 1, 1);
}

Object::~Object()
{
}

Matrix Object::getWorldMatrix()
{
	return this->worldMatrix;
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
	this->scaling += scale;
}

void Object::setScale(Vector3 scale)
{
	this->scaling = scale;
}

void Object::updateWorldMatrix()
{
	this->worldMatrix = DirectX::XMMatrixScalingFromVector(this->scaling) * DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(this->rotation)) * XMMatrixTranslationFromVector(this->translation);
}

std::string Object::getName()
{
	return this->name;
}


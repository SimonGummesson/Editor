#include "../Headers/Object.hpp"

Object::Object(std::string name)
{
	this->name = name;
	this->worldMatrix = XMMatrixIdentity();
	this->translation = { 0, 0, 0 };
	this->rotation = { 0, 0, 0 };
	this->scaling = { 1, 1, 1 };
}

Object::~Object()
{

}

XMMATRIX &Object::getWorldMatrix()
{
	return this->worldMatrix;
}

void Object::translate(DirectX::XMVECTOR translation)
{
	this->translation += translation;
}

void Object::setTranslation(XMVECTOR translation)
{
	this->translation = translation;
}

void Object::rotate(XMVECTOR rotation)
{
	this->rotation += rotation;
}

void Object::setRotation(XMVECTOR rotation)
{
	this->rotation = rotation;
}

void Object::scale(XMVECTOR scale)
{
	this->scaling += scale;
}

void Object::setScale(XMVECTOR scale)
{
	this->scaling = scale;
}

void Object::updateWorldMatrix()
{
	this->worldMatrix = XMMatrixScalingFromVector(this->scaling) * XMMatrixRotationRollPitchYawFromVector(this->rotation) * XMMatrixTranslationFromVector(this->translation);
}

void Object::update()
{

}

std::string Object::getName()
{
	return this->name;
}


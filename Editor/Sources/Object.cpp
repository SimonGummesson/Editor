#include "../Headers/Object.hpp"

Object::Object(std::string name)
{
	this->name = name;
	this->worldMatrix = DirectX::XMMatrixIdentity();
}

Object::~Object()
{

}

DirectX::XMMATRIX &Object::getWorldMatrix()
{
	return this->worldMatrix;
}

void Object::translate(DirectX::XMVECTOR translation)
{
	this->worldMatrix = this->worldMatrix + DirectX::XMMatrixTranslationFromVector(translation);
}

void Object::scale(DirectX::XMVECTOR scale)
{
	this->worldMatrix = this->worldMatrix * DirectX::XMMatrixScalingFromVector(scale);
}

void Object::update()
{

}

std::string Object::getName()
{
	return this->name;
}


#include "..\Headers\TestObject.hpp"

TestObject::TestObject(std::string name) : Object(name)
{
	
}

TestObject::~TestObject()
{

}

void TestObject::update(float dt)
{
	rotate({ XM_PI / 10.f * dt, 0.f, 0.f});
	updateWorldMatrix();
}

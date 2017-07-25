#include "Object.hpp"

class TestObject : public Object
{
public:
	TestObject(std::string name);
	~TestObject();
	void update(float dt) override;
private:
	
};
#include "Object.hpp"

class StaticObject : public Object
{
public:
	StaticObject(std::string name);
	~StaticObject();
	void update(float dt) override;
private:

};
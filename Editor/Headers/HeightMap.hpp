#include "Object.hpp"

class HeightMap : public Object
{
public:
	HeightMap(std::string name);
	~HeightMap();
	void update(float dt) override;
private:

};
#pragma once
#include"Game\GameObject.h"

class WoodBlock :public GameObject
{
public:
	WoodBlock();
	~WoodBlock() {}

	void Deserialize(json11::Json& jObj)override;

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override;
	void Draw3D()override;

private:

};
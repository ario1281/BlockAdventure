#pragma once
#include"Game\GameObject.h"

class FieldBase : public GameObject
{
public:
	FieldBase();
	~FieldBase() {}

	bool Update(const std::list<shared_ptr<GameObject>> *obj)override;
	void Draw3D()override;

};
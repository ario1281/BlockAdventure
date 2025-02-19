#pragma once
#include"Game\GameObject.h"

class TitleLogo : public GameObject
{
public:
	TitleLogo();
	~TitleLogo() {}

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override { return true; }
	void Draw2D()override;

};
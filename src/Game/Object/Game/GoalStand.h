#pragma once
#include"Game\GameObject.h"

class GoalStand :public GameObject
{
public:
	GoalStand();
	~GoalStand() {}

	void Deserialize(json11::Json& jObj)override;

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override;

	bool GoalStaging();

	void Draw3D()override;

private:
	shared_ptr<DirectSoundManager>	m_sBGM = nullptr;

	std::string						m_putTag = "";
};

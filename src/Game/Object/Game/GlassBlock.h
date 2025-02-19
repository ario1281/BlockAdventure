#pragma once
#include"Game\GameObject.h"

class GlassBlock :public GameObject
{
public:
	GlassBlock();
	~GlassBlock() {}

	void Deserialize(json11::Json& jObj)override;

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override;
	void Draw3D()override;

private:
	shared_ptr<DirectSoundManager>	m_sSE = nullptr;
};
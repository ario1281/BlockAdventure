#include"WoodBlock.h"

WoodBlock::WoodBlock()
{
	m_tag = "Field";
	m_name = "WoodBlock";
	m_spModel = ResFac.GetModel("data\\field\\wood\\wood.x");
	m_mWorld.CreateIdentity();
}

void WoodBlock::Deserialize(json11::Json& jObj)
{
	GameObject::Deserialize(jObj);
}

bool WoodBlock::Update(const std::list<shared_ptr<GameObject>>* obj)
{
	if (!GIMMICK.GetFlag()) { return true; }
	GIMMICK.Rotation(m_mWorld);

	return true;
}

void WoodBlock::Draw3D()
{
	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

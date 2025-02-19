#include"GlassBlock.h"

GlassBlock::GlassBlock()
{
	m_tag = "Field";
	m_name = "GlassBlock";
	m_spModel = ResFac.GetModel("data\\field\\glass\\glass.x");
	m_sSE = ResFac.GetSound("data\\field\\glass\\glass_break.wav");
	m_mWorld.CreateIdentity();
}

void GlassBlock::Deserialize(json11::Json& jObj)
{
	GameObject::Deserialize(jObj);
}

bool GlassBlock::Update(const std::list<shared_ptr<GameObject>>* obj)
{
	if (!GIMMICK.GetFlag()) { return true; }
	GIMMICK.Rotation(m_mWorld);

	return true;
}

void GlassBlock::Draw3D()
{
	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

#include"FieldBase.h"

FieldBase::FieldBase()
{
	m_tag = "Field";
	m_name = "FieldBase";
	m_spModel = ResFac.GetModel("data\\field\\field.x");
	m_mWorld.CreateScaling(25.0f, 25.0f, 25.0f);
}

bool FieldBase::Update(const std::list<shared_ptr<GameObject>>* s_obj)
{
	if (!GIMMICK.GetFlag()) { return true; }
	GIMMICK.Rotation(m_mWorld);

	return true;
}

void FieldBase::Draw3D()
{
	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

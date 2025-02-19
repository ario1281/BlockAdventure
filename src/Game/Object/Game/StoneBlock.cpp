#include"StoneBlock.h"

StoneBlock::StoneBlock()
{
	m_tag = "Field";
	m_name = "StoneBlock";
	m_spModel = ResFac.GetModel("data\\field\\stone\\stone.x");
	//m_sSE = ResFac.GetSound("data\\field\\stone\\stone_drag.wav");
	m_mWorld.CreateTranslation(0, -2, 0);

}

void StoneBlock::Deserialize(json11::Json& jObj)
{
	GameObject::Deserialize(jObj);
}

bool StoneBlock::Update(const std::list<shared_ptr<GameObject>>* s_obj)
{
	if (GIMMICK.GetFlag()) { GIMMICK.Rotation(m_mWorld); }
	else { Falling(s_obj); }

	return true;
}

void StoneBlock::Draw3D()
{
	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

void StoneBlock::Falling(const std::list<shared_ptr<GameObject>>* s_obj)
{
	m_Fall += m_axelFall;
	if (m_Fall > m_maxFall) { m_Fall = m_maxFall; }
	Vector3 vFall(0, -m_Fall, 0);
	//　判定部分
	for (auto& obj = s_obj->begin(); obj != s_obj->end(); obj++)
	{
		auto o = obj->get();
		if (o->GetTag() != "Field") { continue; }

		Vector3 pos = m_mWorld.GetPos();
		JudgeRay(o->GetModel(), o->GetWorldMatrix(), Vector3(0, -m_Fall, 0), pos, 100.0f);
	}
	vFall = Vector3(0, -m_Fall, 0);
	//　落下処理
	m_mWorld.Move(vFall);
}

void StoneBlock::JudgeRay(
	const LPD3DXMESH	&s_lpMesh,
	const Matrix		&s_mTarget,
	const Vector3		&s_vDir,
	Vector3				&s_vPos,
	float				jdgLen)
{
	////　軽量化(レイ判定をする範囲を決める)
	//Vector3 vlen = s_vPos - s_mTarget.GetPos();
	//float len = vlen.Length();
	//if (len > jdgLen) { return; }

	float dist = 0.0f;
	bool hit = false;
	Vector3 vpos = Vector3(s_vPos.x, s_vPos.y, s_vPos.z);
	Vector3 vdir = s_vDir;
	vdir.Normalize();
	JUDGE.JudgingLayToMesh(
		s_lpMesh, s_mTarget, vpos, vdir, &hit, NULL, &dist, NULL);
	if (!hit || dist > m_Dist) { return; }

	m_Fall = 0.0f;
	s_vPos.y += m_Dist - dist;
}

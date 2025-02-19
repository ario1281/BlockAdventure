#include"Switch.h"

Switch::Switch()
{
	m_tag		= "Field";
	m_name		= "Switch";
	m_spModel	= ResFac.GetModel("data\\field\\switch\\switch.x");
	m_sSE		= ResFac.GetSound("data\\field\\switch\\rotation.wav");

	m_mWorld.CreateIdentity();

	//　初期位置
	#pragma region 矢印方向の確定

	m_dirTag = "Right";
	float radian;
	if (m_dirTag == "Front") {
		radian = D3DXToRadian(RIGHT_ANGLE * 0);
	}
	if (m_dirTag == "Right") {
		radian = D3DXToRadian(RIGHT_ANGLE * 1);
	}
	if (m_dirTag == "Back") {
		radian = D3DXToRadian(RIGHT_ANGLE * 2);
	}
	if (m_dirTag == "Left") {
		radian = D3DXToRadian(RIGHT_ANGLE * -1);
	}
	m_mWorld.RotateY(radian);

	#pragma endregion
	#pragma region 設置位置の確定(6方向)

	m_putTag = "Down";
	if (m_putTag == "Down") {
		m_mWorld.RotateZ(D3DXToRadian(RIGHT_ANGLE * 0));
	}
	if (m_putTag == "Front") {
		m_mWorld.RotateX(D3DXToRadian(RIGHT_ANGLE * -1));
	}
	if (m_putTag == "Right") {
		m_mWorld.RotateZ(D3DXToRadian(RIGHT_ANGLE * 1));
	}
	if (m_putTag == "Back") {
		m_mWorld.RotateX(D3DXToRadian(RIGHT_ANGLE * 1));
	}
	if (m_putTag == "Left") {
		m_mWorld.RotateZ(D3DXToRadian(RIGHT_ANGLE * -1));
	}
	if (m_putTag == "Up") {
		m_mWorld.RotateX(D3DXToRadian(RIGHT_ANGLE * 2));
	}

	#pragma endregion

	//　回転する向き
	m_vAxis.TransNormal(m_mWorld);

	m_mWorld.MoveLocal(2, -5.5f, 0);
}

void Switch::Deserialize(json11::Json& jObj)
{
	GameObject::Deserialize(jObj);
}

bool Switch::Update(const std::list<shared_ptr<GameObject>>* s_obj)
{
	Rotation(s_obj);
	return true;
}

void Switch::Draw3D()
{
	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

void Switch::Rotation(const std::list<shared_ptr<GameObject>>* s_obj)
{
	for (auto& obj = s_obj->begin(); obj != s_obj->end(); obj++)
	{
		auto o = obj->get();
		if (o->GetTag() != "Player") { continue; }

		auto pos = o->GetWorldMatrix().GetPos();
		bool hit = false;
		JUDGE.JudgingLayToMesh(m_spModel->GetMesh(), m_mWorld,
			pos, Vector3(0, -1, 0), &hit, NULL, NULL, NULL);
		if (hit && !GIMMICK.GetFlag())
		{
			GIMMICK.SetFlag(true);
			m_rot = 90.0f;
			GIMMICK.SetAxis(m_vAxis);
		}
	}

	//　スイッチが押されたとき
	if (!GIMMICK.GetFlag()) { return; }

	#pragma region 回転処理

	if (m_rot >= m_rAng) {
		m_rot1 = m_rAng;
		m_rot -= m_rAng;
		m_sSE->Play(true);
	}
	if (m_rot < m_rAng)
	{
		m_rot1 = m_rot;
		m_rot -= m_rot1;
		m_sSE->Stop();
		GIMMICK.SetFlag(false);
	}
	GIMMICK.SetAngle(m_rot1);

	GIMMICK.Rotation(m_mWorld);

	#pragma endregion

}

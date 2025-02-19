#include"GoalStand.h"

GoalStand::GoalStand()
{
	m_tag = "Field";
	m_name = "GoalStand";
	m_spModel = ResFac.GetModel("data\\field\\goal\\goal.x");
	m_sBGM = ResFac.GetSound("data\\field\\goal\\congrats.wav");
	
	m_mWorld.CreateIdentity();

	//　初期位置
	//　設置位置の確定(６方向)
	m_putTag = "Down";
	if (m_putTag == "Down")	{ m_mWorld.RotateZ(D3DXToRadian(RIGHT_ANGLE * 0)); }
	if (m_putTag == "Front") { m_mWorld.RotateX(D3DXToRadian(RIGHT_ANGLE * -1)); }
	if (m_putTag == "Right") { m_mWorld.RotateZ(D3DXToRadian(RIGHT_ANGLE * 1)); }
	if (m_putTag == "Back") { m_mWorld.RotateX(D3DXToRadian(RIGHT_ANGLE * 1)); }
	if (m_putTag == "Left") { m_mWorld.RotateZ(D3DXToRadian(RIGHT_ANGLE * -1)); }
	if (m_putTag == "Up") { m_mWorld.RotateX(D3DXToRadian(RIGHT_ANGLE * 2)); }

	m_mWorld.MoveLocal(-2, -5.5f, 0);
}

void GoalStand::Deserialize(json11::Json& jObj)
{
	GameObject::Deserialize(jObj);
}

bool GoalStand::Update(const std::list<shared_ptr<GameObject>>* s_obj)
{
	if (TASK.GetGoalFlag()) {
		GoalStaging();

		return true;
	};

	if (GIMMICK.GetFlag()) {
		GIMMICK.Rotation(m_mWorld);
		return true;
	}

	for (auto& obj = s_obj->begin(); obj != s_obj->end(); obj++)
	{
		auto o = obj->get();
		if (o->GetTag() != "Player") { continue; }

		bool hit = false;
		float dist = 0.0f;
		JUDGE.JudgingLayToMesh(
			m_spModel.get()->GetMesh(),
			o->GetWorldMatrix(),
			m_mWorld.GetPos(),
			Vector3(0, -1, 0),
			&hit, nullptr,
			&dist, nullptr);
		if (hit && dist < 1.5f) { TASK.SetGoalFlag(true); }
	}
}

bool GoalStand::GoalStaging()
{
	m_sBGM->Play();
	return true;
}

void GoalStand::Draw3D()
{
	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

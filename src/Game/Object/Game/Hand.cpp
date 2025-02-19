#include"Hand.h"

Hand::Hand()
{
	m_tag = "";
	m_name = "Hand";
	m_spModel = ResFac.GetModel("data\\field\\human\\hand.x");

}

bool Hand::Update(const std::list<shared_ptr<GameObject>>* obj)
{
	if (TASK.GetGoalFlag()) {
		//�@�~�܂��Ă��鎞�̎�̓���
		m_vShake.x = m_vOffset.x + cos(D3DXToRadian(m_triAng * 4)) / 32.0f;
		m_triAng++;
		return true;
	}

	auto owner = m_wpOwner.lock();
	m_vShake = m_vOffset;
	if (owner) { m_mWorld = owner->GetWorldMatrix(); }

	//�@�X�C�b�`�������ꂽ�Ƃ�
	if (GIMMICK.GetFlag()) { return true; }

	if (owner->GetIsKey())
	{
		if (!m_flg) { m_triAng = 0; }
		m_flg = true;

		//�@�ړ����Ă��鎞�̎�̓���
		m_vShake.z = m_vOffset.z + sin(D3DXToRadian(m_triAng * 4)) / 4.0f;
		m_triAng++;
	}
	else
	{
		if (m_flg) { m_triAng = 0; }
		m_flg = false;

		//�@�~�܂��Ă��鎞�̎�̓���
		m_vShake.x = m_vOffset.x + cos(D3DXToRadian(m_triAng * 4)) / 32.0f;
		m_triAng++;
	}
	return true;
}
void Hand::Draw3D()
{
	Matrix mWorld;

	////�@�E��`��
	mWorld = Matrix();
	mWorld.Move(m_vShake);
	mWorld = mWorld * m_mWorld;

	D3D.SetWorldMatrix(&mWorld);
	m_spModel->Draw();


	////�@����`��
	mWorld = Matrix();
	mWorld.Move(m_vShake.x, 0, m_vShake.z);
	mWorld.Inverse();
	mWorld.Move(0, m_vShake.y, 0);
	mWorld = mWorld * m_mWorld;

	D3D.SetWorldMatrix(&mWorld);
	m_spModel->Draw();
}

#pragma once
#include"Game\GameObject.h"

class StoneBlock :public GameObject
{
public:
	StoneBlock();
	~StoneBlock() {}

	void Deserialize(json11::Json& jObj)override;

	bool Update(const std::list<shared_ptr<GameObject>>* s_obj)override;
	void Draw3D()override;

	void Falling(const std::list<shared_ptr<GameObject>>* s_obj);

	void JudgeRay(
		const LPD3DXMESH& s_lpMesh,	//�@�E[in]			��������郁�b�V��
		const Matrix& s_mTarget,	//�@�E[in]			���b�V���̈ʒu
		const Vector3& s_vDir,	//�@�E[in]			��^�[�Q�b�g�̕����x�N�g��
		Vector3& s_vPos,	//�@�E[in / out]	�ʒu
		float				jdgLen		//�@�E[in]			���C��������鋗��
	);

private:
	shared_ptr<DirectSoundManager>	m_sSE = nullptr;

	float						m_Fall = 0.0f;

	//�@�萔
	const float					m_Dist = 0.5f;
#ifdef _DEBUG
	const float					m_axelFall = 0.0375f;
	const float					m_maxFall = 1.5f;
#else
	const float					m_axelFall = 0.0013f;
	const float					m_maxFall = 0.1f;
#endif // _DEBUG
};
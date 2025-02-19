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
		const LPD3DXMESH& s_lpMesh,	//　・[in]			判定をするメッシュ
		const Matrix& s_mTarget,	//　・[in]			メッシュの位置
		const Vector3& s_vDir,	//　・[in]			被ターゲットの方向ベクトル
		Vector3& s_vPos,	//　・[in / out]	位置
		float				jdgLen		//　・[in]			レイ判定をする距離
	);

private:
	shared_ptr<DirectSoundManager>	m_sSE = nullptr;

	float						m_Fall = 0.0f;

	//　定数
	const float					m_Dist = 0.5f;
#ifdef _DEBUG
	const float					m_axelFall = 0.0375f;
	const float					m_maxFall = 1.5f;
#else
	const float					m_axelFall = 0.0013f;
	const float					m_maxFall = 0.1f;
#endif // _DEBUG
};
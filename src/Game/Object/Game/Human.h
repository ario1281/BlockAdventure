#pragma once
#include"Game\GameObject.h"

class Human : public GameObject
{
public:
	Human();
	~Human() {}

	void Deserialize(json11::Json &jObj)override;

	virtual bool Update(const std::list<shared_ptr<GameObject>> *obj)override;
	virtual void Draw3D()override;

	void ImGui();

	bool Moving(Vector3& vec, const std::list<shared_ptr<GameObject>>* s_obj = nullptr);
	bool TurnAround(Vector3 &vec);
	void Falling(const std::list<shared_ptr<GameObject>>* s_obj);



	void JudgeRay(
		const LPD3DXMESH&	s_lpMesh,	//　・[in]			判定をするメッシュ
		const Matrix&		s_mTarget,	//　・[in]			メッシュの位置
		const Vector3&		s_vDir,		//　・[in]			レイの方向
		Vector3&			s_vPos,		//　・[in / out]	レイを飛ばす位置
		bool&				s_hit,		//　・[out]			当たったかどうか
		float&				s_dist);	//　・[out]			当たるまでの距離
	
	void JudgeRayTest(
		const LPD3DXMESH&	s_lpMesh,	//　・[in]			判定をするメッシュ
		const Matrix&		s_mTarget,	//　・[in]			メッシュの位置
		const Vector3&		s_vDir,		//　・[in]			レイの方向
		Vector3&			s_vPos,		//　・[in / out]	レイを飛ばす位置
		const char*			coord = "x");

	void JudgeRayWall(
		const LPD3DXMESH& s_lpMesh,
		const Matrix& s_mTarget,
		const Vector3& s_vDir,
		Vector3& s_vPos);

	void JudgeSphereTriangle(
		const LPD3DXMESH & s_lpMesh,
		const Matrix &s_mTarget,
		Vector3 &s_vPos,
		float s_rad = 1.0f);

	bool GetIsKey() { return m_isKey; }
	void SetOwner(const shared_ptr<InputComponent>& spInput) {
		m_wpInput = spInput;
	}

private:
	weak_ptr<InputComponent> m_wpInput;

	bool						m_isKey;
	float						m_AngX		= 0.0f;
	float						m_Fall		= 0.0f;

	const float					m_Dist = 0.75f;

#ifdef _DEBUG
	const float					m_sMove = 0.25f;
	const float					m_sTurn = 10.0f;
	const float					m_axelFall = 0.015f;
	const float					m_maxFall = 0.75f;
#else
	const float					m_sMove = 0.5f;
	const float					m_sTurn = 0.5f;
	const float					m_axelFall = 0.0013f;
	const float					m_maxFall = 0.1f;
#endif // _DEBUG
};

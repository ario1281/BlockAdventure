#pragma once

class GimmickManager
{
public:
	bool GetFlag() { return m_rFlg; }
	void SetFlag(bool s_Flg) { m_rFlg = s_Flg; }

	void SetAxis(const Vector3& s_axis) { m_vAxis = s_axis; }
	void SetAngle(float s_angle) { m_rAng = s_angle; }

	void Rotation(Matrix& s_mWorld) {
		s_mWorld.RotateAxis(m_vAxis, D3DXToRadian(-m_rAng));
	}

	~GimmickManager() {}

private:
	Vector3			m_vAxis;

	float			m_rAng = 0.0f;	//　どれ位回すか(全体)

	bool						m_rFlg = false;
#pragma region シングルトン
public:
	static GimmickManager& GetInstance() {
		static GimmickManager Instance;
		return Instance;
	}

private:
	GimmickManager() {}
#pragma endregion

};

#define GIMMICK GimmickManager::GetInstance()
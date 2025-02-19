#pragma once

class JudgeManager
{
public:
	void TransformInverse(
		const Matrix& s_mInv,
		Vector3 &s_vPos,
		Vector3 &s_vDir);

	//　レイ判定
	void JudgingLayToMesh(
		const LPD3DXMESH	&lpMesh,	//　・[in]	判定をするメッシュ
		const Matrix		&mTarget,	//　・[in]	メッシュのマトリックス
		const Vector3		&vPos,		//　・[in]	レイの発射位置
		const Vector3		&vDir,		//　・[in]	レイの発射方向
		bool				*pHit,		//	・[out] メッシュに当たったか判定
		DWORD				*pPolyNo,	//　・[out] 当たったポリゴンのナンバー
		float				*pDist,		//　・[out] レイの位置からMメッシュまでの長さ
		DWORD				*pHitCnt);	//　・[out] 一発のレイにヒットした回数

	//　球判定
	bool JudgeSphere(
		const Vector3		&vTarget,	//　・[in]	ターゲットの位置
		const Vector3		&vBase,		//　・[in]	被ターゲットの位置
		float				Dist);		//　・[in]	出れくらいの長さで判定するか


	~JudgeManager() {}
private:

#pragma region シングルトン
public:
	static JudgeManager& GetInstance() {
		static JudgeManager Instance;
		return Instance;
	}
private:
	JudgeManager() {}
#pragma endregion

};

#define JUDGE JudgeManager::GetInstance()
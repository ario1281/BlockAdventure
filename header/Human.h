#pragma once;
#include"define.h"

class Human
{
public:
	Human(const D3DXVECTOR3 *sPos);
	~Human();

	void Update();
	bool Moving(const D3DXMATRIX *s_RotMat);
	bool Falling();

	void Render3D();

	//// アクセサー
	//　ゲッター
	inline const D3DXMATRIX* MatGetter() { return &Mat; };
	inline const D3DXMATRIX* RotMatGetter() { return &RotMat; };
	inline const D3DXMATRIX* TransMatGetter() { return &TransMat; };
	inline const bool* FlagGetter() { return &Flg; };
	inline const D3DXVECTOR3* DirVecGetter() { return &dirVec; };;

	//　セッター
	inline void FallSetter(const float s_Fall) { Fall = s_Fall; };
	inline void FlagSetter(const bool s_Flg) { Flg = s_Flg; };
	inline void MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat);

private:

	void GetMoveVec(D3DXVECTOR3* g_Vec, const D3DXVECTOR3* dirVec, const D3DXMATRIX* YowRotMat);

	const float
		axelFall = 0.0013f,
		maxFall = 0.1f;

	XFILE Mesh;
	D3DXMATRIX Mat;
	D3DXMATRIX TransMat, RotMat;
	D3DXVECTOR3 dirVec;

	XFILE h_Mesh;
	D3DXMATRIX l_Mat, r_Mat;
	D3DXMATRIX h_TransMat;

	float Ang, h_Ang;
	int h_rand;
	bool h_Flg;

	bool Flg = 0, crsrFlg;
	float Fall;
	D3DXVECTOR3 p_dirVec;
};

void Human::MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat)
{
	if (s_Mat != nullptr)		Mat = *s_Mat;
	if (s_RotMat != nullptr)	RotMat = *s_RotMat;
	if (s_TransMat != nullptr)	TransMat = *s_TransMat;
}
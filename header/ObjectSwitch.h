#pragma once
#include"define.h"

#define ROTATION_ANG	90.0f

#ifdef _DEBUG
#define LITTLE_ROT_ANG	0.5f
#else
#define LITTLE_ROT_ANG	0.25f
#endif // _DEBUG


class ObjectSwitch
{
public:
	ObjectSwitch(const D3DXVECTOR3* sPos, char ID, int NumID, int dirID_1, int dirID_2);
	~ObjectSwitch();

	bool PreparateRotation();
	bool StageRotation();
	bool AddToRotation(D3DXVECTOR3 *s_axisVec, D3DXMATRIX *s_RotMat);

	void Render3D();

	inline const LPD3DXMESH MeshGetter() { return Mesh.lpMesh; };
	inline const D3DXMATRIX* MatGetter() { return &Mat; };
	inline const D3DXVECTOR3* axisVecGetter() { return &axisVec; };
	inline const bool* FlagGetter() { return &Flg; };

	inline void MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat);
	inline void axisVecSetter(D3DXVECTOR3* s_Vec) { axisVec = *s_Vec; };
	inline void FlagSetter(const bool s_Flg) { Flg = s_Flg; };

private:
	XFILE Mesh;
	D3DXMATRIX Mat, RotMat, TransMat;
	D3DXVECTOR3 Pos;
	char objID;
	int objNo;

	// ステージ
	float stgAng;
	D3DXMATRIX stgRotMat;
	D3DXVECTOR3 axisVec;
	bool Flg;

	//　サウンド
	LPDIRECTSOUNDBUFFER8	rotSnd;
	LPDIRECTSOUND3DBUFFER8	rot3DSnd;
	int Vol;
};

void ObjectSwitch::MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat)
{
	if (s_Mat != nullptr)		Mat = *s_Mat;
	if (s_RotMat != nullptr)	RotMat = *s_RotMat;
	if (s_TransMat != nullptr)	TransMat = *s_TransMat;
}
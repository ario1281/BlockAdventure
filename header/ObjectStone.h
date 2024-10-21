#pragma once
#include"define.h"

class ObjectStone
{
public:
	ObjectStone(const D3DXVECTOR3* sPos, char ID, int NumID);
	~ObjectStone();

	void Update();
	bool Falling();

	void Render3D();

	inline const LPD3DXMESH MeshGetter() { return Mesh.lpMesh; };
	inline const D3DXMATRIX* MatGetter() { return &Mat; };
	inline const D3DXMATRIX* RotMatGetter() { return &RotMat; };
	inline const D3DXMATRIX* TransMatGetter() { return &TransMat; };
	inline const int* ObjectIDGetter() { return &objNo; };
	inline const float* FallGetter() { return &Fall; };
	inline const bool* FlagGetter() { return &Flg; };

	inline void FallSetter(float s_Fall) { Fall = s_Fall; };
	inline void FlagSetter(bool s_Flg) { Flg = s_Flg; };

	inline void MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat);

private:
	XFILE Mesh;
	D3DXMATRIX Mat, RotMat, TransMat;
	char objID;
	int objNo;

	bool Flg;
	float Fall;
	const float
		axelFall = 0.0025f,
		maxFall = 0.2f;
};

void ObjectStone::MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat)
{
	if (s_Mat != nullptr)		Mat = *s_Mat;
	if (s_RotMat != nullptr)	RotMat = *s_RotMat;
	if (s_TransMat != nullptr)	TransMat = *s_TransMat;
}
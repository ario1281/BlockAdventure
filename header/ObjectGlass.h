#pragma once
#include"define.h"

class ObjectGlass
{
public:
	ObjectGlass(const D3DXVECTOR3* sPos, char ID, int NumID);
	~ObjectGlass();

	bool Breaking();

	void Render3D();

	inline const LPD3DXMESH MeshGetter() { return Mesh.lpMesh; };
	inline const D3DXMATRIX* MatGetter() { return &Mat; };
	inline const D3DXMATRIX* RotMatGetter() { return &RotMat; };

	inline void FlagSetter(bool s_Flg) { Flg = s_Flg; };
	inline void MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat);

private:
	XFILE Mesh;
	LPDIRECT3DTEXTURE9 Tex;
	D3DXMATRIX Mat, RotMat, TransMat;
	D3DXVECTOR3 Pos;
	char objID;
	int objNo;

	bool Flg , sndFlg;


	//　サウンド
	LPDIRECTSOUNDBUFFER8	brkSnd;
	LPDIRECTSOUND3DBUFFER8	brk3DSnd;
	int Vol;
};

void ObjectGlass::MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat)
{
	if (s_Mat != nullptr)		Mat = *s_Mat;
	if (s_RotMat != nullptr)	RotMat = *s_RotMat;
	if (s_TransMat != nullptr)	TransMat = *s_TransMat;
}
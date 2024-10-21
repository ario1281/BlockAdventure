#pragma once
#include"define.h"

class ObjectGoal
{
public:
	ObjectGoal(const D3DXVECTOR3* sPos, char ID, int NumID, char dirID);
	~ObjectGoal();

	bool Update();
	void Render2D();
	void Render3D();

	inline const LPD3DXMESH MeshGetter() { return Mesh.lpMesh; };
	inline const D3DXMATRIX* MatGetter() { return &Mat; };
	inline const bool* FlagGetter() { return &g_Flg; };
	inline const LPDIRECTSOUNDBUFFER8 SoundGetter() { return g_Snd; };

	inline void FlagSetter(bool s_Flg) { g_Flg = s_Flg; };
	inline void MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat);
	inline void SoundSetter(const LPDIRECTSOUNDBUFFER8 s_Snd) { g_Snd = s_Snd; };

private:
	XFILE Mesh;
	D3DXMATRIX Mat, RotMat, TransMat;
	D3DXVECTOR3 Pos;
	char objID;
	int objNo;


	LPDIRECT3DTEXTURE9 logoTex;
	static const int logoNum = 5;
	int logoCnt, zeroCnt;

	TextureLength l_TexLen;
	D3DXMATRIX logoMat[logoNum];
	float logoSym;
	int triAng;
	bool g_Flg;

	static const int c_matNum = 50;
	D3DXMATRIX crkMat[c_matNum];
	LPDIRECT3DTEXTURE9 s_crkTex[c_matNum];
	int c_cntTex, c_cnt[c_matNum], rndTex[c_matNum];
	bool crkFlg[c_matNum];

	static const int c_texNum = 6;
	LPDIRECT3DTEXTURE9 g_crkTex[c_texNum];
	TextureLength c_TexLen[3];

	LPDIRECTSOUNDBUFFER8	g_Snd;
	LPDIRECTSOUND3DBUFFER8	g_3DSnd;
	bool g_sndFlg;
	int Vol;
};

void ObjectGoal::MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat)
{
	if (s_Mat != nullptr)		Mat = *s_Mat;
	if (s_RotMat != nullptr)	RotMat = *s_RotMat;
	if (s_TransMat != nullptr)	TransMat = *s_TransMat;
}
#pragma once
#include"define.h"

#include"SceneBase.h"
#include"SceneManager.h"

#include"ObjectBack.h"

class SceneOption :public SceneBase
{
public:
	SceneOption(const LPDIRECTSOUNDBUFFER8 s_snd, const LPDIRECTSOUND3DBUFFER8 s_3Dsnd);
	~SceneOption();

	bool Update();
	void Render3D();
	void Render2D();

private: 
	void Cursor();
	void Volume(int *g_Vol);
	void BackScene();

	ObjectBack *bg;

	stTexture op_Msg;

	LPDIRECT3DTEXTURE9 slctTex;
	static const int slctImg = 2;
	D3DXVECTOR2 slctPos;
	const int slctMax = 4;
	float symLen;
	TextureLength s_TexLen;
	float shake = 20.0f;
	float triAng = 0;

	// マウス設定
	stTexture op_Mouse;
	stTexture op_1;

	// バックボタン
	stTexture op_back;
	D3DXMATRIX sclMat;

	// BGM,SE設定
	stTexture op_BGM;
	stTexture op_SE;
	static const int volNum = 3;
	VERTEX volVer[volNum][4];
	stTexture op_Vol;

	int crsrNum, slctNum;
	const int crsrMax = 4;
	int w_keyFlg[2], h_keyFlg;

	int cnt_hold_l, cnt_hold_r;

	// 影
	D3DXMATRIX tmpShadowMat;

	//　サウンド
	LPDIRECTSOUNDBUFFER8	bgmSnd;
	LPDIRECTSOUND3DBUFFER8	bgm3DSnd;

	LPDIRECTSOUNDBUFFER8	chSnd;
	LPDIRECTSOUND3DBUFFER8	ch3DSnd;

	LPDIRECTSOUNDBUFFER8	decSnd;
	LPDIRECTSOUND3DBUFFER8	dec3DSnd;
	int Vol, volBGM, volSE;
	const int volMax = 100;
};
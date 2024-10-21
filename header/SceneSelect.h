#pragma once
#include"define.h"

#include"SceneBase.h"
#include"SceneManager.h"

#include"ObjectBack.h"

#include"SceneLoad.h"

#define STAGE_LOGO (SCRH / 2.0f + SCRH / 16.0f)

//�@�X�e�[�W�̉摜
struct StageState
{
	VERTEX				Ver[4];
	LPDIRECT3DTEXTURE9	Tex;
	D3DXMATRIX			Mat, TransMat, RotMat;
};

class SceneSelect :public SceneBase
{
public:
	SceneSelect(const LPDIRECTSOUNDBUFFER8 s_snd, const LPDIRECTSOUND3DBUFFER8 s_3Dsnd);
	~SceneSelect();
	bool Update();
	void Render3D();
	void Render2D();
	void SetCamera();

private:
	ObjectBack* bg;

	int stgNum;
	const int minNum = 1, maxNum = 3;
	bool keyFlg, renFlg;
	enum Determine { op_Key, op_Crsr };
	int det_1;

	// �X�e�[�W���S
	const int stgNo = 3;
	
	LPDIRECT3DTEXTURE9	s_logoTex;
	TextureLength		s_logoTexLen;
	LPDIRECT3DTEXTURE9	s_NumTex;
	TextureLength		s_NumTexLen;

	D3DXMATRIX			s_logoTransMat;
	D3DXMATRIX			*s_logoMat;
	D3DXVECTOR3			*s_logoPos;

	const float RotAng = 0.05f;
	D3DXVECTOR3			stgPos;


	// �X�e�[�W
	StageState* stgState;
	D3DXMATRIX sumRotMat;

	// �X�e�[�W�Z���N�g���S
	stTexture stg_slct;

	// �I�v�V�������S
	stTexture op_logo;
	float shake = 40.0f;
	float BaseY;
	float sinAng = 0;

	// �e
	D3DXMATRIX tmpShadowMat;

	// �t�F�[�h
	stTexture fade;
	int aFlg = false;

	//�@�T�E���h
	LPDIRECTSOUNDBUFFER8	bgmSnd;
	LPDIRECTSOUND3DBUFFER8	bgm3DSnd;

	LPDIRECTSOUNDBUFFER8	chSnd;
	LPDIRECTSOUND3DBUFFER8	ch3DSnd;

	LPDIRECTSOUNDBUFFER8	decSnd;
	LPDIRECTSOUND3DBUFFER8	dec3DSnd;
	int Vol;
};
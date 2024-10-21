#pragma once
#include"define.h"
#include "SceneBase.h"
#include"SceneManager.h"
#include"TaskManager.h"

class SceneTitle:public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();
	bool Update();
	void Render2D();

private:
	stTexture tit_back;

	stTexture tit_logo;
	D3DXMATRIX l_TransMat, l_ScaleMat;

	stTexture st_mes;
	float shake = 40.0f;
	float baseY;
	float triAng = 0;

	// �e
	D3DXMATRIX tmpShadowMat;

	//�@����
	stTexture quit_logo;
	D3DXMATRIX q_sclMat;
	float q_triAng = 0;
	bool q_keyFlg, stFlg;
	int quitFlg;
	int qf_cnt;
	int q_fade;

	//�@�t�F�[�h
	stTexture fade;
	int aFlg = false;

	//�@�T�E���h
	LPDIRECTSOUNDBUFFER8	bgmSnd;
	LPDIRECTSOUND3DBUFFER8	bgm3DSnd;

	LPDIRECTSOUNDBUFFER8	stSnd;
	LPDIRECTSOUND3DBUFFER8	st3DSnd;

	int Vol;
};
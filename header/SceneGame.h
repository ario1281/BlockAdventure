#pragma once
#include<vector>

#include"define.h"
#include "SceneBase.h"
#include"SceneManager.h"
#include"JudgeManager.h"

#include"ObjectBack.h"
#include"ObjectStage.h"
#include"ObjectWood.h"
#include"ObjectStone.h"
#include"ObjectGlass.h"
#include"ObjectSwitch.h"
#include"ObjectGoal.h"

#include"Human.h"
#include"Cursor.h"

#define LEN_POS (0.5f + tmpScale * 0.5f)
#define R_ERROR ','
#define JDG_LEN 0.5f

//　座標
struct Stage
{
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

struct objInfo
{
	int No;
	char ID;
	D3DXVECTOR3 Pos;
};

class SceneGame :public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	bool Update();
	void Render2D();
	void Render3D();
	void SetCamera();

private:
	void CameraJudge(float *re_Len, bool *re_Flg, const LPD3DXMESH lpTarMesh, const D3DXMATRIX *tarMat, const D3DXVECTOR3 *hitPos, const D3DXVECTOR3 *dirVec);

	Cursor* crsr;
	Human* player;
	ObjectBack* bg;

	ObjectStage* stgBase;
	std::vector<ObjectWood*> arr_wd;
	std::vector<ObjectStone*> arr_st;
	std::vector<ObjectGlass*> arr_gl;
	std::vector<ObjectSwitch*> arr_sw;
	ObjectGoal* goal;


	D3DXMATRIX camRotMat, camXMat, camYMat;
	D3DXMATRIX plyMat;

	//　ステージ
	unsigned long int objNo = 0;
	bool ply_Flg = false, fin_Flg = false;

	//　カメラ判定
	VERTEX jdgVer[4];

	//　サウンド
	LPDIRECTSOUNDBUFFER8	bgmSnd;
	LPDIRECTSOUND3DBUFFER8	bgm3DSnd;
	int Vol;
};
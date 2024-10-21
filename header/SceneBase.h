#pragma once
#include "define.h"

class SceneBase
{
public:
	~SceneBase();
	virtual bool Update();
	virtual void Render3D();
	virtual void Render2D();

	virtual void SetCamera();

	virtual void SetLight();

	void Frame();

protected:
	static bool scnKeyFlg;
	D3DXVECTOR3 camPos, camLook, camHead;
	D3DLIGHT9 Light;
	
	enum Scene
	{
		Load,
		Title,
		Select,
		Game,
		Option
	};
	static int c_cnt;
	//　フェード
	static int f_a;
};


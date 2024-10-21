#pragma once
#include "SceneBase.h"

class SceneLoad:public SceneBase
{
public:
	SceneLoad(int s_Scene);
	~SceneLoad();
	bool Update();
private:
	int SceneNum;
};
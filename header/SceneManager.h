#pragma once
#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include"SceneOption.h"
#include "SceneGame.h"
#include "SceneLoad.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void NowScene();
	void ChangeScene(SceneBase *NewScene);

private:
	SceneBase *Scene;
};
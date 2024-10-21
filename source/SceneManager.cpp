#include"../header/SceneManager.h"


SceneManager::SceneManager()
{
	Scene = nullptr;
}
SceneManager::~SceneManager()
{
	if (Scene != nullptr)	delete Scene;
}

void SceneManager::NowScene()
{
	if (Scene != nullptr)	Scene->Frame();
}

void SceneManager::ChangeScene(SceneBase*NewScene)
{
	if (Scene != nullptr)	delete Scene;

	Scene = NewScene;
}
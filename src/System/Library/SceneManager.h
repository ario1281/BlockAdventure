#pragma once
#include"Game/SceneBase.h"

class SceneManager
{
public:
	void NowScene() {
		if (Scene != nullptr) { Scene->Frame(); }
	}
	void ChangeScene(SceneBase *NewScene) {
		if (Scene != nullptr) { delete Scene; }
		Scene = NewScene;
	}

	~SceneManager() {
		if (Scene != nullptr) { delete Scene; }
	}

	SceneBase*& GetScene() { return Scene; }

private:
	SceneBase *Scene = nullptr;

#pragma region ƒVƒ“ƒOƒ‹ƒgƒ“
public:
	static SceneManager& GetInstance() {
		static SceneManager Instance;
		return Instance;
	}
private:
	SceneManager() {}
#pragma endregion

};

#define SCENE SceneManager::GetInstance()
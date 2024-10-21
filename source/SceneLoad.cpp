#include"../header/define.h"
#include"../header/SceneManager.h"

extern SceneManager			scnManager;

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color);

SceneLoad::SceneLoad(int s_Scene)
{
	SceneNum = s_Scene;
}
SceneLoad::~SceneLoad()
{

}


bool SceneLoad::Update()
{
	switch (SceneNum)
	{
	case Title:
		scnManager.ChangeScene(new SceneTitle());
		break;

	case Select:
		scnManager.ChangeScene(new SceneSelect(nullptr, nullptr));
		break;

	case Game:
		scnManager.ChangeScene(new SceneGame());
		break;

	case Option:
		break;

	default:
		break;
	}

	return false;
}
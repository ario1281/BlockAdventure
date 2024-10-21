#include "../header/define.h"
#include "../header/SceneManager.h"

extern	SceneManager	scnManager;

bool SceneBase::scnKeyFlg = false;
int SceneBase::f_a = 0;
int SceneBase::c_cnt = 0;

DWORD NowTime, PrevTime;
int cntFrame, fps, fpsCnt = -1;
const int scd = 1;

SceneBase::~SceneBase()
{
}

bool SceneBase::Update()
{
	return true;
}

void SceneBase::Render3D()
{

}
void SceneBase::Render2D()
{

	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);



	lpSprite->End();

	///////////
	//　fps　//
	///////////
	//RECT rcFps = { 0,0,0,0 };
	//char fpsText[200];
	//sprintf_s(fpsText, sizeof(fpsText), "%3dfps", fps);

	//lpFont->DrawText(NULL, fpsText, -1, &rcFps, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
}

void SceneBase::SetCamera()
{
	D3DXMATRIX mView, mProj;

	// 視点行列の設定
	D3DXMatrixLookAtLH(&mView,
		&D3DXVECTOR3(0.0f, 0.0f, -10.0f),	// カメラの位置
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// カメラの視点
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)	// カメラの頭の方向
	);

	// 投影行列の設定
	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(60), (float)SCRW / (float)SCRH, 1.0f, 1000.0f);

	//行列設定
	lpD3DDevice->SetTransform(D3DTS_VIEW, &mView);
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);

}

void SceneBase::SetLight()
{
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(2.0f, -1.0f, 1.0f);

	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;
	Light.Ambient.r = 0.125f;
	Light.Ambient.g = 0.125f;
	Light.Ambient.b = 0.125f;
	Light.Specular.r = 1.0f;
	Light.Specular.g = 1.0f;
	Light.Specular.b = 1.0f;

	lpD3DDevice->SetLight(0, &Light);
	lpD3DDevice->LightEnable(0, TRUE);
}



void SceneBase::Frame()
{

	fpsCnt++;
	if (fpsCnt / (60 * scd) < 60) return;

	if (Update() == false) return;
	
	//NowTime = timeGetTime();
	//cntFrame++;
	//if ((NowTime - PrevTime) >= 1000)
	//{
	//	PrevTime = NowTime;
	//	fps = cntFrame;
	//	cntFrame = 0;
	//}

	lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(105, 105, 105), 1.0f, 0);
	
	lpD3DDevice->BeginScene();

	SetCamera();

	Render3D();
	Render2D();

	lpD3DDevice->EndScene();


	lpD3DDevice->Present(NULL, NULL, NULL, NULL);
	
}
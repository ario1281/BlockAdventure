#include"main.h"
#include"Game\Scene\GameScene.h"
#include"Game\Scene\TitleScene.h"


/*===================================================================
	メイン
===================================================================*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpParam, int nCmdshow)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	/*===================================================================
		ウィンドウ作成、Direct3D初期化など
	===================================================================*/
	if (!APP.Init(hInst, nCmdshow, SCRW, SCRH)) return 0;

	/*===================================================================
		ループ
	===================================================================*/
	APP.Loop();

	/*===================================================================
		解放
	===================================================================*/
	APP.Release();

	// COM解放
	CoUninitialize();

	return 0;
}


//　アプリケーション初期設定
bool System::Init(HINSTANCE _hInst, int _cmdShow, int _w, int _h)
{
	APP.m_hInst = _hInst;

	/*===================================================================
		フルスクリーン確認
	===================================================================*/
	bool bFullScreen = false;
	if (MessageBox(nullptr, "フルスクリーンにしますか？", "確認",
		MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
	{
		bFullScreen = true;
	}

	/*===================================================================
		ウィンドウ作成
	===================================================================*/
	if (!m_Window.Create(_hInst, _cmdShow, _w, _h, "Window"))
	{
		MessageBox(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}
	
	/*===================================================================
		Direct3D初期化
	===================================================================*/
	std::string errMsg;
	if (!D3D.Init(m_Window.GetHWnd(), _w, _h, bFullScreen, errMsg))
	{
		MessageBox(m_Window.GetHWnd(), errMsg.c_str(), "Direct3D初期化失敗",
			MB_OK | MB_ICONSTOP);
		return false;
	}

	// 基本的なレンダーステート設定
	D3D.SetDefaultState();

	if (!DX_AUDIO.Init(m_Window.GetHWnd(), errMsg))
	{
		MessageBox(m_Window.GetHWnd(), errMsg.c_str(), "DirectSound初期化失敗",
			MB_OK | MB_ICONSTOP);
		return false;
	}

	/*===================================================================
		imgui初期化
	===================================================================*/
	//IMGUI.Init();

	return true;
}

//　アプリケーション終了
void System::Release()
{
	D3D.Release();
	//IMGUI.Release();
	m_Window.Release();
}

//　メインループ
void System::Loop()
{
	DWORD baseTime = timeGetTime();
	int count = 0;

	srand(baseTime);

	m_Scene = new TitleScene();
	SCENE.ChangeScene(m_Scene);

	m_Light.Init();
	m_Light.Diffuse(191.25f, 191.25f, 191.25f);
	m_Light.Ambient(0, 0, 0);
	m_Light.Direction(2.0f, -1.0f, 1.0f);
	m_Light.Set(0);

	CURSOR.Init();

	while (true)
	{
		//　ゲーム終了指定
		if (m_EndFlg) { break; }

		/*=========================================
			ウィンドウ関係の処理
		=========================================*/
		if (!m_Window.IsCreated()) { break; }
		m_Window.ProcessMessage();

		/*=========================================
			ゲーム処理の関係
		=========================================*/
		//　処理開始時間
		DWORD st = timeGetTime();

		/*=========================================
			ゲーム処理の関係
		=========================================*/
		SCENE.NowScene();

		if (m_sceneFlg) {
			SCENE.ChangeScene(m_Scene);
			m_sceneFlg = false;
		}

		/*=========================================
			Fps制御
		=========================================*/
		//　処理終了時間
		DWORD et = timeGetTime();
		// Fps制御
		if (et - st < 16) {
			Sleep(16 - (et - st));	// 速すぎたら待つ
		}

		//　FPS計測
		count++;
		if (st - baseTime >= 1000) {
			m_Fps = (count * 1000) / (st - baseTime);
			baseTime = st;
			count = 0;
		}
		m_FrameCnt++;
	}
}

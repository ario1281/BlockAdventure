#pragma once
#include <windows.h>
#include <stdio.h>

#include "System\Library\define.h"
#include"Game\GameWindow.h"
#include"System\Library\LightManager.h"

class System
{
public:
	bool Init(HINSTANCE _hInst, int _cmdChow, int _w, int _h);
	void Loop();
	void Release();

	void EndGame() { m_EndFlg = true; }

	#pragma region 取得系

	const HWND& GetHWnd() const { return m_Window.GetHWnd(); }
	const bool GetAnyKeyFlag() const { return m_Window.GetAnyKeyFlag(); }
	const bool GetMouseFlag() const { return m_Window.GetMouseFlag(); }
	bool GetSceneFlag() { return m_sceneFlg; }
	void SetSceneFlag() { m_sceneFlg = false; }

	void SetChangeScene(SceneBase* s_scene) {
		m_sceneFlg = true;
		m_Scene = s_scene;
	}

	#pragma endregion


	/*=================================================
		公開データ
	=================================================*/

	//　ゲームウィンドウクラス
	GameWindow m_Window;
	//　シーンクラス
	SceneBase *m_Scene = nullptr;

	LightManager m_Light;

	/*=================================================
		グローバルデータ
	=================================================*/
	HINSTANCE	m_hInst = nullptr;
	DWORD		m_FrameCnt = 0;
	int			m_Fps = 0;

private:
	bool m_EndFlg = false;
	bool m_sceneFlg = false;


public:
	static System &GetInstance() {
		static System Instance;
		return Instance;
	}

private:
	System() {}

};

#define APP System::GetInstance()
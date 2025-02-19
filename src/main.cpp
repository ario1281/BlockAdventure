#include"main.h"
#include"Game\Scene\GameScene.h"
#include"Game\Scene\TitleScene.h"


/*===================================================================
	���C��
===================================================================*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpParam, int nCmdshow)
{
	// ���������[�N��m�点��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM������
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	/*===================================================================
		�E�B���h�E�쐬�ADirect3D�������Ȃ�
	===================================================================*/
	if (!APP.Init(hInst, nCmdshow, SCRW, SCRH)) return 0;

	/*===================================================================
		���[�v
	===================================================================*/
	APP.Loop();

	/*===================================================================
		���
	===================================================================*/
	APP.Release();

	// COM���
	CoUninitialize();

	return 0;
}


//�@�A�v���P�[�V���������ݒ�
bool System::Init(HINSTANCE _hInst, int _cmdShow, int _w, int _h)
{
	APP.m_hInst = _hInst;

	/*===================================================================
		�t���X�N���[���m�F
	===================================================================*/
	bool bFullScreen = false;
	if (MessageBox(nullptr, "�t���X�N���[���ɂ��܂����H", "�m�F",
		MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
	{
		bFullScreen = true;
	}

	/*===================================================================
		�E�B���h�E�쐬
	===================================================================*/
	if (!m_Window.Create(_hInst, _cmdShow, _w, _h, "Window"))
	{
		MessageBox(nullptr, "�E�B���h�E�쐬�Ɏ��s", "�G���[", MB_OK);
		return false;
	}
	
	/*===================================================================
		Direct3D������
	===================================================================*/
	std::string errMsg;
	if (!D3D.Init(m_Window.GetHWnd(), _w, _h, bFullScreen, errMsg))
	{
		MessageBox(m_Window.GetHWnd(), errMsg.c_str(), "Direct3D���������s",
			MB_OK | MB_ICONSTOP);
		return false;
	}

	// ��{�I�ȃ����_�[�X�e�[�g�ݒ�
	D3D.SetDefaultState();

	if (!DX_AUDIO.Init(m_Window.GetHWnd(), errMsg))
	{
		MessageBox(m_Window.GetHWnd(), errMsg.c_str(), "DirectSound���������s",
			MB_OK | MB_ICONSTOP);
		return false;
	}

	/*===================================================================
		imgui������
	===================================================================*/
	//IMGUI.Init();

	return true;
}

//�@�A�v���P�[�V�����I��
void System::Release()
{
	D3D.Release();
	//IMGUI.Release();
	m_Window.Release();
}

//�@���C�����[�v
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
		//�@�Q�[���I���w��
		if (m_EndFlg) { break; }

		/*=========================================
			�E�B���h�E�֌W�̏���
		=========================================*/
		if (!m_Window.IsCreated()) { break; }
		m_Window.ProcessMessage();

		/*=========================================
			�Q�[�������̊֌W
		=========================================*/
		//�@�����J�n����
		DWORD st = timeGetTime();

		/*=========================================
			�Q�[�������̊֌W
		=========================================*/
		SCENE.NowScene();

		if (m_sceneFlg) {
			SCENE.ChangeScene(m_Scene);
			m_sceneFlg = false;
		}

		/*=========================================
			Fps����
		=========================================*/
		//�@�����I������
		DWORD et = timeGetTime();
		// Fps����
		if (et - st < 16) {
			Sleep(16 - (et - st));	// ����������҂�
		}

		//�@FPS�v��
		count++;
		if (st - baseTime >= 1000) {
			m_Fps = (count * 1000) / (st - baseTime);
			baseTime = st;
			count = 0;
		}
		m_FrameCnt++;
	}
}

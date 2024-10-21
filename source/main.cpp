#include"../header/define.h"
#include"../header/SceneManager.h"
#include"../header/SceneBase.h"
#include"../header/Cursor.h"

#pragma region �O���[�o���ϐ�

LPDIRECT3D9				lpD3D;
D3DPRESENT_PARAMETERS	d3dpp;

HWND hwnd;

LPDIRECT3DDEVICE9		lpD3DDevice;
LPD3DXSPRITE lpSprite;
LPD3DXFONT lpFont;

LPDIRECTSOUND8 lpDSound;
LPDIRECTSOUNDBUFFER lpSPrimary;
LPDIRECTSOUND3DLISTENER8 lpSListener;

SceneManager scnManager;

TaskManager		taskManager;
TextureManager	texManager;
MeshManager		meshManager;
SoundManager	sndManager;


bool gameFullScreen;

#pragma endregion

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr)	return;

	lpAdd->Release();
	lpAdd = nullptr;
};

void Draw2D(const D3DXMATRIX* s_TransMat, const LPDIRECT3DTEXTURE9 DrawTex, const RECT* Rect, const D3DXVECTOR3* Center, const D3DXCOLOR color)
{
	lpSprite->SetTransform(s_TransMat);
	lpSprite->Draw(DrawTex, Rect, Center, NULL, color);
}

void DSoundInit()
{
	DirectSoundCreate8(NULL, &lpDSound, NULL);

	lpDSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	DSBUFFERDESC dsbdesc;
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);

	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	lpDSound->CreateSoundBuffer(&dsbdesc, &lpSPrimary, NULL);

	WAVEFORMATEX pcmwf;
	ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX));
	pcmwf.wFormatTag = WAVE_FORMAT_PCM;
	pcmwf.nChannels = 2;
	pcmwf.nSamplesPerSec = 44100;
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
	pcmwf.wBitsPerSample = 16;
	lpSPrimary->SetFormat(&pcmwf);

	CoInitialize(NULL);

	lpSPrimary->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&lpSListener);
	lpSListener->SetRolloffFactor(0.5f, DS3D_IMMEDIATE);
}


LRESULT APIENTRY WndFunc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//PostQuitMessage(WM_QUIT);
			return 0;
		}
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdParam, int nCmdshow)
{
	MSG msg;

	WNDCLASS wc;
	char szAppName[] = "Generic Game SDK Window";

	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WndFunc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		szAppName,
		"Block Adventure",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCRW, SCRH,
		NULL, NULL, hInst,
		NULL);

	if (!hwnd)return FALSE;

	ShowWindow(hwnd, nCmdshow);
	UpdateWindow(hwnd);
	SetFocus(hwnd);

	gameFullScreen = false;

	//�@�X�N���[���m�F
	if (MessageBox(NULL, "�t���X�N���[���ŋN�����܂����H",
		"�N���m�F", MB_YESNO | MB_DEFBUTTON2) == IDYES)
	{
		gameFullScreen = true;
	}

	if (gameFullScreen)
	{
		ShowCursor(FALSE);
	}
	else
	{
		RECT rc = { 0,0,SCRW,SCRH };

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		SetWindowPos(hwnd, NULL, 50, 50, rc.right - rc.left, rc.bottom - rc.top,
			SWP_SHOWWINDOW | SWP_NOZORDER);
	}

	// Direct3D �I�u�W�F�N�g���쐬
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		// �I�u�W�F�N�g�쐬���s
		MessageBox(NULL, "Direct3D �̍쐬�Ɏ��s���܂����B", "ERROR", MB_OK | MB_ICONSTOP);
		// �I������
		return 0;
	}
	int adapter;

	// �g�p����A�_�v�^�ԍ�
	adapter = D3DADAPTER_DEFAULT;

	// �E�C���h�E�̍쐬�����������̂ŁADirect3D ������������
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	// Direct3D �������p�����[�^�̐ݒ�
	if (gameFullScreen)
	{
		// �t���X�N���[�� : �قƂ�ǂ̃A�_�v�^�ŃT�|�[�g����Ă���t�H�[�}�b�g���g�p
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		// �E�C���h�E : ���݂̉�ʃ��[�h���g�p
		D3DDISPLAYMODE disp;
		// ���݂̉�ʃ��[�h���擾
		lpD3D->GetAdapterDisplayMode(adapter, &disp);
		d3dpp.BackBufferFormat = disp.Format;
	}
	// �\���̈�T�C�Y�̐ݒ�
	d3dpp.BackBufferWidth = SCRW;
	d3dpp.BackBufferHeight = SCRH;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if (!gameFullScreen)
	{
		// �E�C���h�E���[�h
		d3dpp.Windowed = 1;
	}

	// Z �o�b�t�@�̎����쐬
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//�ޯ��ޯ̧��ۯ��\�ɂ���(GetDC���\�ɂȂ�)
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// �f�o�C�X�̍쐬 - T&L HAL
	if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
	{
		// ���s�����̂� HAL �Ŏ��s
		if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
		{
			// ���s�����̂� REF �Ŏ��s
			if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_REF, hwnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
			{
				// ���ǎ��s����
				MessageBox(NULL,
					"DirectX9���������ł��܂���B\n���Ή��̃p�\�R���Ǝv���܂��B",
					"ERROR", MB_OK | MB_ICONSTOP);
				lpD3D->Release();
				// �I������
				return 0;
			}
		}
	}

	// �����_�����O�E�X�e�[�g��ݒ�
	// Z �o�b�t�@�L����->�O��֌W�̌v�Z�𐳊m�ɂ��Ă����
	lpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�u�����f�B���O�L����
	lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// �A���t�@�u�����f�B���O���@��ݒ�
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �����_�����O���̃A���t�@�l�̌v�Z���@�̐ݒ�
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// �e�N�X�`���̐F���g�p
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	// ���_�̐F���g�p
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	// �����_�����O���̐F�̌v�Z���@�̐ݒ�
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//���ʃJ�����O
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �t�B���^�ݒ�
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// ���C�g
	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	lpD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// ���_�t�H�[�}�b�g�̐ݒ�
	lpD3DDevice->SetFVF(FVF_VERTEX);

	timeBeginPeriod(1);

	DSoundInit();

	// �X�v���C�g�쐬
	D3DXCreateSprite(lpD3DDevice, &lpSprite);
	lpSprite->OnResetDevice();

	// �t�H���g�쐬
	D3DXCreateFont(lpD3DDevice, 30, 30, FW_REGULAR, NULL, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "�l�r �S�V�b�N", &lpFont);

	lpFont->OnResetDevice();

	// �錾������
	scnManager.ChangeScene(new SceneTitle());

	srand(timeGetTime());

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			scnManager.NowScene();
		}
	}

	// �Q�[���Ɋւ���I������ ---------------------------

	SafeRelease(lpSPrimary);
	SafeRelease(lpDSound);

	SafeRelease(lpSprite);	// �X�v���C�g
	SafeRelease(lpFont);	// �t�H���g


	timeEndPeriod(1);

	// Direct3D �I�u�W�F�N�g�����
	SafeRelease(lpD3DDevice);
	SafeRelease(lpD3D);

	return (int)msg.wParam;
}
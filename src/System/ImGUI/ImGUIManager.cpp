#include"../Library/define.h"
#include"ImGUIManager.h"


void ImGUIManager::Init()
{
	//　環境設定
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	static_cast<ImGuiIO&>(m_IO) = ImGui::GetIO();
	(void)m_IO;

	//　imguiのスタイルカラー
#if DARK
	ImGui::StyleColorsDark();
#else
	ImGui::StyleColorsClassic();
#endif // DARK

	SetUpPlatform();
	SetUpDirect3D();
}
bool ImGUIManager::NewFrame()
{
	CreateFontTexture();
	return false;
}

bool ImGUIManager::SetUpPlatform()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&m_TPS)) { return false; }
	if (!QueryPerformanceCounter((LARGE_INTEGER*)&m_Time)) { return false; }

	#pragma region ソリューションプラットホーム
	////　　　　　設定　　　　　////
	m_IO.BackendPlatformName = "ImGui_win32";
	m_IO.BackendFlags
		|= ImGuiBackendFlags_HasSetMousePos + ImGuiBackendFlags_HasMouseCursors;
	m_IO.ImeWindowHandle = APP.GetHWnd();

	//　Keyの割り当て
	m_IO.KeyMap[ImGuiKey_Tab]			= VK_TAB;
	m_IO.KeyMap[ImGuiKey_LeftArrow]		= VK_LEFT;
	m_IO.KeyMap[ImGuiKey_RightArrow]	= VK_RIGHT;
	m_IO.KeyMap[ImGuiKey_UpArrow]		= VK_UP;
	m_IO.KeyMap[ImGuiKey_DownArrow]		= VK_DOWN;
	m_IO.KeyMap[ImGuiKey_PageUp]		= VK_PRIOR;
	m_IO.KeyMap[ImGuiKey_PageDown]		= VK_NEXT;
	m_IO.KeyMap[ImGuiKey_Home]			= VK_HOME;
	m_IO.KeyMap[ImGuiKey_End]			= VK_END;
	m_IO.KeyMap[ImGuiKey_Insert]		= VK_INSERT;
	m_IO.KeyMap[ImGuiKey_Delete]		= VK_DELETE;
	m_IO.KeyMap[ImGuiKey_Backspace]		= VK_BACK;
	m_IO.KeyMap[ImGuiKey_Space]			= VK_SPACE;
	m_IO.KeyMap[ImGuiKey_Enter]			= VK_RETURN;
	m_IO.KeyMap[ImGuiKey_Escape]		= VK_ESCAPE;
	m_IO.KeyMap[ImGuiKey_KeyPadEnter]	= VK_RETURN;
	m_IO.KeyMap[ImGuiKey_A]				= 'A';
	m_IO.KeyMap[ImGuiKey_C]				= 'C';
	m_IO.KeyMap[ImGuiKey_V]				= 'V';
	m_IO.KeyMap[ImGuiKey_X]				= 'X';
	m_IO.KeyMap[ImGuiKey_Y]				= 'Y';
	m_IO.KeyMap[ImGuiKey_Z]				= 'Z';

	#pragma endregion

	return true;
}
void ImGUIManager::SetUpDirect3D()
{
	m_IO.BackendRendererName = "ImGui_DX9";
	m_IO.BackendFlags
		|= ImGuiBackendFlags_RendererHasVtxOffset;

	DEVICE->AddRef();
}

bool ImGUIManager::CreateFontTexture()
{
	unsigned char* px;
	int width, height;
	int bpp;	//　BytePerPixel
	m_IO.Fonts->GetTexDataAsRGBA32(&px, &width, &height, &bpp);

	if (FAILED(DEVICE->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_FontTex, NULL))) {
		return false;
	}

	D3DLOCKED_RECT lock_rcTex;
	if (FAILED(m_FontTex->LockRect(0, &lock_rcTex, NULL, 0))) { return false; }

	for (int y = 0; y < height; y++)
	{
		memcpy((UCHAR*)lock_rcTex.pBits + lock_rcTex.Pitch * y,
			px + (width * bpp) * y,
			(width * bpp));
	}

	m_FontTex->UnlockRect(0);
	m_IO.Fonts->TexID = (ImTextureID)m_FontTex;

	return false;
}

void ImGUIManager::PlatfomeNewFrame()
{
	IM_ASSERT(m_IO.Fonts->IsBuilt() &&
		"Font atlas not built! It is generally built by the renderer backend.\
		Missing call to renderer _NewFrame() function? e.g.\
		ImGui_ImplOpenGL3_NewFrame().");

	//　ディスプレイのサイズ
	RECT rc = { 0,0,0,0 };
	GetClientRect(APP.GetHWnd(), &rc);
	m_IO.DisplaySize
		= ImVec2((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));

	//　時間
	INT64 current_time = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
	m_IO.DeltaTime = (float)(current_time - m_Time) / m_TPS;
	m_Time = current_time;

	//　キーの設定
	m_IO.KeyCtrl	= (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	m_IO.KeyShift	= (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	m_IO.KeyAlt		= (GetKeyState(VK_MENU) & 0x8000) != 0;
	m_IO.KeySuper	= false;

	UpdateMousePos();

	ImGuiMouseCursor crsr
		= m_IO.MouseDrawCursor ?
		ImGuiMouseCursor_None :
		ImGui::GetMouseCursor();
	if (m_MouseCrsr != crsr)
	{
		m_MouseCrsr = crsr;

	}
}

bool ImGUIManager::UpdateMouseCursor()
{
	if (m_IO.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
		return false;
	}

	ImGuiMouseCursor guiCrsr = ImGui::GetMouseCursor();
	if (guiCrsr == ImGuiMouseCursor_None || m_IO.MouseDrawCursor) {
		SetCursor(NULL);
	}
	else {
		LPTSTR crsr = IDC_ARROW;
		switch (guiCrsr) {
		case ImGuiMouseCursor_Arrow:
			crsr = IDC_ARROW;
			break;
		case ImGuiMouseCursor_TextInput:
			crsr = IDC_IBEAM;
			break;
		case ImGuiMouseCursor_ResizeAll:
			crsr = IDC_SIZEALL;
			break;
		case ImGuiMouseCursor_ResizeEW:
			crsr = IDC_SIZEWE;
			break;
		case ImGuiMouseCursor_ResizeNS:
			crsr = IDC_SIZENS;
			break;
		case ImGuiMouseCursor_ResizeNESW:
			crsr = IDC_SIZENESW;
			break;
		case ImGuiMouseCursor_ResizeNWSE:
			crsr = IDC_SIZENWSE;
			break;
		case ImGuiMouseCursor_Hand:
			crsr = IDC_HAND;
			break;
		case ImGuiMouseCursor_NotAllowed:
			crsr = IDC_NO;
			break;
		}
		SetCursor(LoadCursor(NULL, crsr));
	}
	return true;
}

void ImGUIManager::UpdateMousePos()
{
	if (m_IO.WantSetMousePos)
	{
		POINT pos = { (int)m_IO.MousePos.x,(int)m_IO.MousePos.y };
		if (ClientToScreen(APP.GetHWnd(), &pos)) { SetCursorPos(pos.x, pos.y); }
	}

	m_IO.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	POINT pos;
	if (HWND active = GetForegroundWindow())
		if (active == APP.GetHWnd() || IsChild(active, APP.GetHWnd()))
			if (GetCursorPos(&pos) && ScreenToClient(APP.GetHWnd(), &pos))
				m_IO.MousePos = ImVec2((float)pos.x, (float)pos.y);
}

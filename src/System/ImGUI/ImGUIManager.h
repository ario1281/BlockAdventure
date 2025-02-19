#pragma once

#define DARK 0x01

class ImGUIManager
{
public:
	void Init();
	ImGuiIO& GetImGui() { return m_IO; }

	bool NewFrame();

	void Release() {
		ImGui::DestroyContext();
	}

	bool UpdateMouseCursor();
	void UpdateMousePos();

	~ImGUIManager(){}
private:
	bool SetUpPlatform();
	void SetUpDirect3D();

	bool CreateFontTexture();
	void PlatfomeNewFrame();

	ImGuiIO				m_IO;
	
	bool				m_sflg			= false;
	ImVec4				clear_color;

	//Å@Platform
	int					m_TPS			= 0;
	int					m_Time			= 0;
	ImGuiMouseCursor	m_MouseCrsr		= ImGuiMouseCursor_COUNT;

	//Å@Direct3D
	LPDIRECT3DTEXTURE9 m_FontTex = nullptr;

public:
	static ImGUIManager& GetInstance() {
		static ImGUIManager Instance;
		return Instance;
	}
private:
	ImGUIManager() {}
};

#define IMGUI ImGUIManager::GetInstance()
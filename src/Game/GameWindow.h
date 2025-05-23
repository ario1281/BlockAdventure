#pragma once

class GameWindow
{
public:
	HWND GetHWnd() const { return m_hWnd; }

	bool IsCreated() const { return m_hWnd ? true : false; }

	int GetMouseWheelVal()const { return m_MouseWheelVal; }
	bool GetAnyKeyFlag()const { return m_AnyKeyFlg; }
	bool GetMouseFlag()const { return m_MouseFlg; }

	bool Create(HINSTANCE _hInst,int cmdShow, int clientWidth, int clientHeight, const std::string &wndName);

	void Release();

	bool ProcessMessage();

	void SetClientSize(int w, int h);

	~GameWindow() {
		Release();
	}

private:
	HWND	m_hWnd = nullptr;
	bool m_AnyKeyFlg = false;
	bool m_MouseFlg = false;

	static LRESULT CALLBACK CallWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int		m_MouseWheelVal = 0;
};
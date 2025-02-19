#pragma once

class Direct3D
{
public:

	#pragma region 取得系

	//　D3Dオブジェクト取得
	LPDIRECT3D9EX			GetD3D() { return m_lpD3D; }
	//　D3Dデバイス取得
	LPDIRECT3DDEVICE9EX		GetDev() { return m_lpD3DDev; }
	//　D3Dパラメータ取得
	D3DPRESENT_PARAMETERS*	GetD3DPP() { return &m_d3dpp; }
	//　デバイス情報取得
	D3DCAPS9*				GetCaps() { return &m_Caps; }

	//　解像度取得
	int						GetRezoW() const { return m_d3dpp.BackBufferWidth; }
	int						GetRezoH() const { return m_d3dpp.BackBufferHeight; }

	//　スプライト取得
	SpriteManager&			GetSprite() { return *m_spSprite; }

	#pragma endregion

	/*=============================================
		基本的関数
	=============================================*/
	bool Init(HWND _hWnd, int _w, int _h, bool fullscreen, std::string &errMsg);
	void Release();

	void Begin();

	/*=============================================
		ウィンドウ/フルスクリーン切り替え
	=============================================*/
	bool ChangeFullScreenMode();

	#pragma region ステート

	// 基本的なレンダーステート等の設定
	void SetDefaultState();

	// 加算合成モード
	void Blend_Add() {
		// 加算合成モードに切り替える
		m_lpD3DDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	// 半透明合成モード
	void Blend_Alpha() {
		m_lpD3DDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	//　カメラ
	void SetCamera(const CameraManager& cam);
	const CameraManager& GetCamera() { return m_camera; }

	#pragma endregion

	//=============================================
	// 行列セット
	//=============================================
	// ワールド変換行列をセット
	void SetWorldMatrix(const D3DXMATRIX *m) {
		m_lpD3DDev->SetTransform(D3DTS_WORLD, m);
	}


	~Direct3D() {
		Release();
	}

private:
	//　定数
	UINT						m_hz = 60;
	D3DCOLOR					m_bColor = D3DCOLOR_ARGB(255, 0, 0, 255);

	LPDIRECT3D9EX				m_lpD3D = nullptr;
	LPDIRECT3DDEVICE9EX			m_lpD3DDev = nullptr;
	D3DPRESENT_PARAMETERS		m_d3dpp;
	D3DCAPS9					m_Caps;

	LPDIRECT3DSURFACE9			m_OrgRenderTarget = nullptr;
	LPDIRECT3DSURFACE9			m_OrgDepthStencil = nullptr;

	shared_ptr<SpriteManager> m_spSprite;

	CameraManager m_camera;

public:
	static Direct3D &GetInstance() {
		static Direct3D Instance;
		return Instance;
	}

private:
	Direct3D() {}
};

#define D3D Direct3D::GetInstance()
#define DEVICE D3D.GetDev()
#define SPRITE D3D.GetSprite()
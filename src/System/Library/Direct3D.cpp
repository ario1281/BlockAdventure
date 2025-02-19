#include"define.h"
#include"Direct3D.h"

bool Direct3D::Init(HWND _hWnd, int _w, int _h, bool _fsFlg, std::string & _errMsg)
{
	/*=======================================================
		D3Dオブジェクト作成
	=======================================================*/
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &m_lpD3D))) {
		_errMsg = "Direct3Dの初期化に失敗", "Direct3D初期化";
		return false;
	}

	/*=======================================================
		現在の画面モードを取得
	=======================================================*/
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		Release();
		return false;
	}

	/*=======================================================
		デバイス作成のための設定
	=======================================================*/
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.BackBufferCount = 2;

	//　フルスクリーン
	if (_fsFlg) {
		m_d3dpp.Windowed					= false;
		m_d3dpp.BackBufferWidth				= _w;
		m_d3dpp.BackBufferHeight			= _h;
		m_d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
		m_d3dpp.FullScreen_RefreshRateInHz	= m_hz;
	}
	//　ウィンドウ
	else {
		m_d3dpp.Windowed					= true;
		m_d3dpp.BackBufferWidth				= 0;
		m_d3dpp.BackBufferHeight			= 0;
		m_d3dpp.BackBufferFormat			= d3ddm.Format;
	}
	m_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	//　スワップエフェクト
	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.hDeviceWindow			= _hWnd;
	//　Zバッファーの自動生成
	m_d3dpp.EnableAutoDepthStencil = true;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	//　ディスプレイモード設定
	D3DDISPLAYMODEEX dm;
	dm.Width	= _w;
	dm.Height	= _h;
	dm.Format = m_d3dpp.BackBufferFormat;
	dm.ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;
	dm.RefreshRate = m_d3dpp.FullScreen_RefreshRateInHz;
	dm.Size = sizeof(dm);
	/*=======================================================
		デバイスの作成
	=======================================================*/
	DWORD flgs =
		D3DCREATE_HARDWARE_VERTEXPROCESSING |
		D3DCREATE_MULTITHREADED |
		D3DCREATE_FPU_PRESERVE;

	if (FAILED(m_lpD3D->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		_hWnd, flgs, &m_d3dpp, (_fsFlg ? &dm : nullptr), &m_lpD3DDev)))
	{
		_errMsg = "Direct3Dの初期化に失敗", "Direct3D初期化";

		Release();
		return false;
	}

	/*=======================================================
		Caps取得
	=======================================================*/
	m_lpD3DDev->GetDeviceCaps(&m_Caps);

	/*=======================================================
		デフォルトのバックバッファとZバッファ取得しておく
	=======================================================*/
	m_lpD3DDev->GetRenderTarget(0, &m_OrgRenderTarget);
	m_lpD3DDev->GetDepthStencilSurface(&m_OrgDepthStencil);

	/*=======================================================
		デフォルトのビュー行列とプロジェクション行列を設定
	=======================================================*/
	//　ビュー行列作成
	D3DXMATRIX mView;
	D3DXMatrixTranslation(&mView, 0, 0, -10.0f);

	D3DXMatrixInverse(&mView, 0, &mView);
	m_lpD3DDev->SetTransform(D3DTS_VIEW, &mView);

	//　プロジェクト行列作成
	D3DXMATRIX mProj;
	D3DXMatrixPerspectiveFovLH(&mProj,
		D3DXToRadian(60), (float)_w / _h, 0.1f, 2000.f);
	m_lpD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

	//　スプライトの初期化
	m_spSprite = std::make_shared<SpriteManager>();
	m_spSprite->Init();

	return true;
}

void Direct3D::Release()
{
	SafeRelease(m_OrgRenderTarget);
	SafeRelease(m_OrgDepthStencil);

	SafeRelease(m_lpD3DDev);

	SafeRelease(m_lpD3D);
}

bool Direct3D::ChangeFullScreenMode()
{
	if (m_d3dpp.Windowed) {
		m_d3dpp.Windowed					= false;
		m_d3dpp.FullScreen_RefreshRateInHz	= m_hz;
	}
	//　ウィンドウ
	else {
		m_d3dpp.Windowed					= true;
		m_d3dpp.BackBufferWidth				= 0;
		m_d3dpp.BackBufferHeight			= 0;
		m_d3dpp.FullScreen_RefreshRateInHz	= 0;
	}

	//　ディスプレイモード設定
	D3DDISPLAYMODEEX dm;
	dm.Width = m_d3dpp.BackBufferWidth;
	dm.Height = m_d3dpp.BackBufferHeight;
	dm.Format = m_d3dpp.BackBufferFormat;
	dm.ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;
	dm.RefreshRate = m_d3dpp.FullScreen_RefreshRateInHz;
	dm.Size = sizeof(dm);

	if (FAILED(m_lpD3DDev->ResetEx(&m_d3dpp, m_d3dpp.Windowed ? nullptr : &dm)))
		return false;
	return true;
}

void Direct3D::SetDefaultState()
{
	// DirectGraphicsの詳細設定
	m_lpD3DDev->SetRenderState(D3DRS_ZENABLE, TRUE);					// Zバッファ有効
	m_lpD3DDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファに書き込み有効
	m_lpD3DDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	m_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		// アルファブレンド有効
	// 半透明モード
	m_lpD3DDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//頂点法線の自動正規化
	m_lpD3DDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);		

	m_lpD3DDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);	// グローシェーディング
	m_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// カリング CCW = 右回り = 表面のみ描画
	// アルファ値個別計算
	m_lpD3DDev->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	m_lpD3DDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
	m_lpD3DDev->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	m_lpD3DDev->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);

	// テクスチャのα値のブレンディング方法の設定
	// テクスチャα合成処理の方法
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);    // 以下の引数の成分を乗算する
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);    // テクスチャの色を使用
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);    // 頂点の色を使用
	// 色を合成する方法
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);    // 以下の引数の成分を乗算する
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);    // テクスチャの色を使用
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);    // 頂点の色を使用
	// テクスチャ補間を線形に
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	// テクスチャアドレッシングモード設定
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// ラップモード
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// ラップモード
	// ミップマップ詳細レベルを設定する。
	float fMipBias = 0.0f;
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *((DWORD*)&fMipBias));
	// ライト
	m_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	// スペキュラ
	m_lpD3DDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	// フォグ(霧効果)
	m_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

void Direct3D::SetCamera(const CameraManager& cam)
{
	m_camera = cam;
	m_lpD3DDev->SetTransform(D3DTS_VIEW, &m_camera.GetView());
	m_lpD3DDev->SetTransform(D3DTS_PROJECTION, &m_camera.GetProj());
}

void Direct3D::Begin()
{
	DWORD flags =
		D3DCLEAR_TARGET |	//　ARGB情報
		D3DCLEAR_ZBUFFER |	//　奥行情報
		D3DCLEAR_STENCIL;	//　マスク用情報

	m_lpD3DDev->Clear(0, nullptr, flags, m_bColor, 1.0f, 0);
	m_lpD3DDev->BeginScene(); // 描画開始
}


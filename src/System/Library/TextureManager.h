#pragma once

/*===================================================================
	
	主にテクスチャを読み込み / データ保管

===================================================================*/
class TextureManager
{
public:
	// 読み込み
	// ・filename		…　ファイル名
	// ・戻り値			…　成功:true
	bool Load(const std::string& filename);

	// 解放
	void Release() {
		SafeRelease(m_tex);
	}

	// 自動解放
	~TextureManager() {
		Release();
	}

	// Direct3Dテクスチャを取得
	LPDIRECT3DTEXTURE9 GetTex()const { return m_tex; }
	const RECT& GetRect()const { return m_rc; }

	// 画像情報取得
	const D3DXIMAGE_INFO& GetInfo() { return m_info; }
	const D3DXVECTOR2& GetRectLength() {
		return D3DXVECTOR2(float(m_rc.right - m_rc.left), float(m_rc.bottom - m_rc.top));
	}


private:
	// Direct3Dのテクスチャ
	LPDIRECT3DTEXTURE9	m_tex = nullptr;
	// 画像情報
	D3DXIMAGE_INFO		m_info;
	RECT m_rc; //テクスチャの矩形
};

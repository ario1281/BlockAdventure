#pragma once

class TextureManager;
class Matrix;

class SpriteManager
{
public:
	~SpriteManager(){
		Release();
	}
	void Init();
	void Release();

	//　描画
	bool Begin() {
		if (FAILED(m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND)))
			return false;
		return true;
	}
	void End() { m_lpSprite->End(); }

	void SetTransform(const Matrix &m) {
		m_lpSprite->SetTransform(&m);
	}
	void DrawTex(
		const Matrix			&mat,
		const TextureManager	*tex,
		const RECT				*rc = nullptr,
		D3DCOLOR				color = D3DCOLOR_RGBA(255, 255, 255, 255)
	);

private:
	// スプライトへの命令用デバイス
	LPD3DXSPRITE m_lpSprite = nullptr;
	// 文字表示用フォント
	LPD3DXFONT m_lpFont = nullptr;
	// ↑のフォント情報
	D3DXFONT_DESC m_FontDesc;
};
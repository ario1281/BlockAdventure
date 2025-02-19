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

	//�@�`��
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
	// �X�v���C�g�ւ̖��ߗp�f�o�C�X
	LPD3DXSPRITE m_lpSprite = nullptr;
	// �����\���p�t�H���g
	LPD3DXFONT m_lpFont = nullptr;
	// ���̃t�H���g���
	D3DXFONT_DESC m_FontDesc;
};
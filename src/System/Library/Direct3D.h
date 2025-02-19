#pragma once

class Direct3D
{
public:

	#pragma region �擾�n

	//�@D3D�I�u�W�F�N�g�擾
	LPDIRECT3D9EX			GetD3D() { return m_lpD3D; }
	//�@D3D�f�o�C�X�擾
	LPDIRECT3DDEVICE9EX		GetDev() { return m_lpD3DDev; }
	//�@D3D�p�����[�^�擾
	D3DPRESENT_PARAMETERS*	GetD3DPP() { return &m_d3dpp; }
	//�@�f�o�C�X���擾
	D3DCAPS9*				GetCaps() { return &m_Caps; }

	//�@�𑜓x�擾
	int						GetRezoW() const { return m_d3dpp.BackBufferWidth; }
	int						GetRezoH() const { return m_d3dpp.BackBufferHeight; }

	//�@�X�v���C�g�擾
	SpriteManager&			GetSprite() { return *m_spSprite; }

	#pragma endregion

	/*=============================================
		��{�I�֐�
	=============================================*/
	bool Init(HWND _hWnd, int _w, int _h, bool fullscreen, std::string &errMsg);
	void Release();

	void Begin();

	/*=============================================
		�E�B���h�E/�t���X�N���[���؂�ւ�
	=============================================*/
	bool ChangeFullScreenMode();

	#pragma region �X�e�[�g

	// ��{�I�ȃ����_�[�X�e�[�g���̐ݒ�
	void SetDefaultState();

	// ���Z�������[�h
	void Blend_Add() {
		// ���Z�������[�h�ɐ؂�ւ���
		m_lpD3DDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	// �������������[�h
	void Blend_Alpha() {
		m_lpD3DDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	//�@�J����
	void SetCamera(const CameraManager& cam);
	const CameraManager& GetCamera() { return m_camera; }

	#pragma endregion

	//=============================================
	// �s��Z�b�g
	//=============================================
	// ���[���h�ϊ��s����Z�b�g
	void SetWorldMatrix(const D3DXMATRIX *m) {
		m_lpD3DDev->SetTransform(D3DTS_WORLD, m);
	}


	~Direct3D() {
		Release();
	}

private:
	//�@�萔
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
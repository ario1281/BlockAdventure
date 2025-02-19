#include"define.h"
#include"Direct3D.h"

bool Direct3D::Init(HWND _hWnd, int _w, int _h, bool _fsFlg, std::string & _errMsg)
{
	/*=======================================================
		D3D�I�u�W�F�N�g�쐬
	=======================================================*/
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &m_lpD3D))) {
		_errMsg = "Direct3D�̏������Ɏ��s", "Direct3D������";
		return false;
	}

	/*=======================================================
		���݂̉�ʃ��[�h���擾
	=======================================================*/
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		Release();
		return false;
	}

	/*=======================================================
		�f�o�C�X�쐬�̂��߂̐ݒ�
	=======================================================*/
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.BackBufferCount = 2;

	//�@�t���X�N���[��
	if (_fsFlg) {
		m_d3dpp.Windowed					= false;
		m_d3dpp.BackBufferWidth				= _w;
		m_d3dpp.BackBufferHeight			= _h;
		m_d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
		m_d3dpp.FullScreen_RefreshRateInHz	= m_hz;
	}
	//�@�E�B���h�E
	else {
		m_d3dpp.Windowed					= true;
		m_d3dpp.BackBufferWidth				= 0;
		m_d3dpp.BackBufferHeight			= 0;
		m_d3dpp.BackBufferFormat			= d3ddm.Format;
	}
	m_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	//�@�X���b�v�G�t�F�N�g
	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.hDeviceWindow			= _hWnd;
	//�@Z�o�b�t�@�[�̎�������
	m_d3dpp.EnableAutoDepthStencil = true;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	//�@�f�B�X�v���C���[�h�ݒ�
	D3DDISPLAYMODEEX dm;
	dm.Width	= _w;
	dm.Height	= _h;
	dm.Format = m_d3dpp.BackBufferFormat;
	dm.ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;
	dm.RefreshRate = m_d3dpp.FullScreen_RefreshRateInHz;
	dm.Size = sizeof(dm);
	/*=======================================================
		�f�o�C�X�̍쐬
	=======================================================*/
	DWORD flgs =
		D3DCREATE_HARDWARE_VERTEXPROCESSING |
		D3DCREATE_MULTITHREADED |
		D3DCREATE_FPU_PRESERVE;

	if (FAILED(m_lpD3D->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		_hWnd, flgs, &m_d3dpp, (_fsFlg ? &dm : nullptr), &m_lpD3DDev)))
	{
		_errMsg = "Direct3D�̏������Ɏ��s", "Direct3D������";

		Release();
		return false;
	}

	/*=======================================================
		Caps�擾
	=======================================================*/
	m_lpD3DDev->GetDeviceCaps(&m_Caps);

	/*=======================================================
		�f�t�H���g�̃o�b�N�o�b�t�@��Z�o�b�t�@�擾���Ă���
	=======================================================*/
	m_lpD3DDev->GetRenderTarget(0, &m_OrgRenderTarget);
	m_lpD3DDev->GetDepthStencilSurface(&m_OrgDepthStencil);

	/*=======================================================
		�f�t�H���g�̃r���[�s��ƃv���W�F�N�V�����s���ݒ�
	=======================================================*/
	//�@�r���[�s��쐬
	D3DXMATRIX mView;
	D3DXMatrixTranslation(&mView, 0, 0, -10.0f);

	D3DXMatrixInverse(&mView, 0, &mView);
	m_lpD3DDev->SetTransform(D3DTS_VIEW, &mView);

	//�@�v���W�F�N�g�s��쐬
	D3DXMATRIX mProj;
	D3DXMatrixPerspectiveFovLH(&mProj,
		D3DXToRadian(60), (float)_w / _h, 0.1f, 2000.f);
	m_lpD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

	//�@�X�v���C�g�̏�����
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
	//�@�E�B���h�E
	else {
		m_d3dpp.Windowed					= true;
		m_d3dpp.BackBufferWidth				= 0;
		m_d3dpp.BackBufferHeight			= 0;
		m_d3dpp.FullScreen_RefreshRateInHz	= 0;
	}

	//�@�f�B�X�v���C���[�h�ݒ�
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
	// DirectGraphics�̏ڍאݒ�
	m_lpD3DDev->SetRenderState(D3DRS_ZENABLE, TRUE);					// Z�o�b�t�@�L��
	m_lpD3DDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�ɏ������ݗL��
	m_lpD3DDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	m_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		// �A���t�@�u�����h�L��
	// ���������[�h
	m_lpD3DDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//���_�@���̎������K��
	m_lpD3DDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);		

	m_lpD3DDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);	// �O���[�V�F�[�f�B���O
	m_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// �J�����O CCW = �E��� = �\�ʂ̂ݕ`��
	// �A���t�@�l�ʌv�Z
	m_lpD3DDev->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	m_lpD3DDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
	m_lpD3DDev->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	m_lpD3DDev->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���̃��l�̃u�����f�B���O���@�̐ݒ�
	// �e�N�X�`�������������̕��@
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);    // �ȉ��̈����̐�������Z����
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);    // �e�N�X�`���̐F���g�p
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);    // ���_�̐F���g�p
	// �F������������@
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);    // �ȉ��̈����̐�������Z����
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);    // �e�N�X�`���̐F���g�p
	m_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);    // ���_�̐F���g�p
	// �e�N�X�`����Ԃ���`��
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	// �e�N�X�`���A�h���b�V���O���[�h�ݒ�
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// ���b�v���[�h
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// ���b�v���[�h
	// �~�b�v�}�b�v�ڍ׃��x����ݒ肷��B
	float fMipBias = 0.0f;
	m_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *((DWORD*)&fMipBias));
	// ���C�g
	m_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	// �X�y�L����
	m_lpD3DDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	// �t�H�O(������)
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
		D3DCLEAR_TARGET |	//�@ARGB���
		D3DCLEAR_ZBUFFER |	//�@���s���
		D3DCLEAR_STENCIL;	//�@�}�X�N�p���

	m_lpD3DDev->Clear(0, nullptr, flags, m_bColor, 1.0f, 0);
	m_lpD3DDev->BeginScene(); // �`��J�n
}


#pragma once

/*===================================================================
	
	��Ƀe�N�X�`����ǂݍ��� / �f�[�^�ۊ�

===================================================================*/
class TextureManager
{
public:
	// �ǂݍ���
	// �Efilename		�c�@�t�@�C����
	// �E�߂�l			�c�@����:true
	bool Load(const std::string& filename);

	// ���
	void Release() {
		SafeRelease(m_tex);
	}

	// �������
	~TextureManager() {
		Release();
	}

	// Direct3D�e�N�X�`�����擾
	LPDIRECT3DTEXTURE9 GetTex()const { return m_tex; }
	const RECT& GetRect()const { return m_rc; }

	// �摜���擾
	const D3DXIMAGE_INFO& GetInfo() { return m_info; }
	const D3DXVECTOR2& GetRectLength() {
		return D3DXVECTOR2(float(m_rc.right - m_rc.left), float(m_rc.bottom - m_rc.top));
	}


private:
	// Direct3D�̃e�N�X�`��
	LPDIRECT3DTEXTURE9	m_tex = nullptr;
	// �摜���
	D3DXIMAGE_INFO		m_info;
	RECT m_rc; //�e�N�X�`���̋�`
};

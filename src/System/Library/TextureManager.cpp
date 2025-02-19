#include "TextureManager.h"


bool TextureManager::Load(const std::string& _filename)
{
	Release();

	auto hr = D3DXCreateTextureFromFileEx(
		DEVICE,
		_filename.c_str(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		&m_info,
		nullptr,
		&m_tex);

	if (FAILED(hr)) return false;

	SetRect(&m_rc, 0, 0, m_info.Width, m_info.Height);
	return true;
}


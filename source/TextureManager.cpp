#include"../header/define.h"

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color)
{
	if (W == 0)W = D3DX_DEFAULT;
	if (H == 0)H = D3DX_DEFAULT;
	D3DXCreateTextureFromFileEx(lpD3DDevice, fname, W, H, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, Color, NULL, NULL, lpTex);
}

TextureManager::~TextureManager()
{
	AllDelete();
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(std::string FileName, int Width, int Height, D3DCOLOR COLORKEY)
{
	LPDIRECT3DTEXTURE9 Tmp;
	if (texList.find(FileName) == texList.end())
	{
		LoadTexture(&Tmp, &FileName[0], Width, Height, COLORKEY);
		texList[FileName] = Tmp;
	}
	else Tmp = texList[FileName];

	return Tmp;
}

void TextureManager::AllDelete()
{
	for (auto itr = texList.begin(); itr != texList.end(); ++itr)
	{
		itr->second->Release();
	}
	texList.clear();
}
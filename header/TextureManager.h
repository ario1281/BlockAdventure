#pragma once
#include<string>
#include<map>
#include"define.h"

#define TEXW( tex_W ) (SCRW - (tex_W))
#define TEXH( tex_H ) (SCRH - (tex_H))

struct TextureLength
{
	int w;
	int h;
};

struct stTexture
{
	LPDIRECT3DTEXTURE9 Tex;
	D3DXMATRIX Mat;
	TextureLength Len;
};


class TextureManager
{
public:
	~TextureManager();
	LPDIRECT3DTEXTURE9 GetTexture(std::string FileName, int Width, int Height, D3DCOLOR COLORKEY);
	void AllDelete();

private:
	std::map<std::string, LPDIRECT3DTEXTURE9>texList;
};
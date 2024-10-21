#pragma once
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<d3dx9.h>
#include<time.h>

#include"wavread.h"
#include"TaskManager.h"
#include"TextureManager.h"
#include"SoundManager.h"
#include"MeshManager.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dsound.lib")

#define	SCRW 1280
#define	SCRH 720

#define	FVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

enum objType
{
	Nothing = '0',
	startPos = '1',
	Wood = 'a',
	Stone = 'b',
	Glass = 'c',

	Goal = 'm',
	Switch = 'n'
};
enum Direct
{
	down = '1',
	left = '2',
	rear = '3',
	front = '4',
	right = '5',
	up = '6'
};


struct VERTEX
{
	D3DXVECTOR3	Pos;
	D3DCOLOR	Color;
	D3DXVECTOR2	Tex;
};
struct XFILE
{
	LPD3DXMESH			lpMesh;
	DWORD				NumMaterial;
	D3DMATERIAL9*		Mat;
	LPDIRECT3DTEXTURE9*	Tex;
};

class SoundManager;
class MeshManager;

extern LPDIRECT3DDEVICE9		lpD3DDevice;
extern LPD3DXSPRITE				lpSprite;
extern LPD3DXFONT				lpFont;

extern LPDIRECTSOUND8			lpDSound;
extern LPDIRECTSOUNDBUFFER		lpSPrimary;
extern LPDIRECTSOUND3DLISTENER8 lpSListener;

extern HWND hwnd;

extern TaskManager		taskManager;
extern TextureManager	texManager;
extern MeshManager		meshManager;
extern SoundManager		sndManager;


void Draw2D(const D3DXMATRIX* s_TransMat, const LPDIRECT3DTEXTURE9 DrawTex, const RECT* Rect, const D3DXVECTOR3* Center, const D3DXCOLOR color);

void DrawMesh(XFILE *XFile);

void RectPolygonCreate(VERTEX* ver, float Wid, float Hei, float Scale);
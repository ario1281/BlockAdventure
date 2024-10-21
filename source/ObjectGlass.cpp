#include"../header/ObjectGlass.h"

extern MeshManager		meshManager;

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr) return;
	lpAdd->Release();
	lpAdd = nullptr;
}

ObjectGlass::ObjectGlass(const D3DXVECTOR3* sPos, char ID, int NumID)
{
	objID = ID;
	objNo = NumID;

	Flg = true;

	Mesh = meshManager.GetMesh("data/mat/mesh/glass.x");
	sndManager.GetSound(brkSnd, brk3DSnd, "data/mat/snd/glass_break.wav");

	if (*taskManager.SEGetter() != 0)	Vol = (int)(log10f((float)*taskManager.SEGetter()) * 5000.0f) - 10000;
	else								Vol = -10000;
	brkSnd->SetVolume(Vol);

	Pos = *sPos;
	D3DXMatrixTranslation(&TransMat, Pos.x, Pos.y, Pos.z);
	D3DXMatrixIdentity(&RotMat);

	Mat = RotMat * TransMat;
	sndFlg = false;
}
ObjectGlass::~ObjectGlass()
{
	SafeRelease(brkSnd);
	SafeRelease(brk3DSnd);
}

bool ObjectGlass::Breaking()
{
	if (Flg == false)
	{
		if (sndFlg == false)
		{
			D3DXMatrixIdentity(&Mat);
			brkSnd->Play(0, 0, 0);
			sndFlg = true;
		}
	}

	return true;
}


void ObjectGlass::Render3D()
{
	if(Flg)
	{
		lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
		DrawMesh(&Mesh);
	}
}
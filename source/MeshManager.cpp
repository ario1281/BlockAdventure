#include"../header/MeshManager.h"

void DrawMesh(XFILE *XFile)
{
	for (DWORD i = 0; i < XFile->NumMaterial; i++)
	{
		lpD3DDevice->SetMaterial(&(XFile->Mat[i]));
		lpD3DDevice->SetTexture(0, XFile->Tex[i]);
		XFile->lpMesh->DrawSubset(i);
	}
}

MeshManager::~MeshManager()
{
	AllDelete();
}
void RectPolygonCreate(VERTEX* ver, float Wid, float Hei, float Scale)
{
	ver[0].Pos = D3DXVECTOR3(-(Wid / 2) * Scale, (Hei / 2) * Scale, 0.0f);
	ver[1].Pos = D3DXVECTOR3((Wid / 2) * Scale, (Hei / 2) * Scale, 0.0f);
	ver[2].Pos = D3DXVECTOR3((Wid / 2) * Scale, -(Hei / 2) * Scale, 0.0f);
	ver[3].Pos = D3DXVECTOR3(-(Wid / 2) * Scale, -(Hei / 2) * Scale, 0.0f);

	ver[0].Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	ver[1].Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	ver[2].Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	ver[3].Color = D3DCOLOR_ARGB(255, 255, 255, 255);

	ver[0].Tex = D3DXVECTOR2(0.0f / Hei, 0.0f / Wid);
	ver[1].Tex = D3DXVECTOR2(Hei / Hei, 0.0f / Wid);
	ver[2].Tex = D3DXVECTOR2(Hei / Hei, Wid / Wid);
	ver[3].Tex = D3DXVECTOR2(0.0f / Hei, Wid / Wid);
}

XFILE MeshManager::GetMesh(std::string FileName)
{
	XFILE XFile;

	if (MeshList.find(FileName) == MeshList.end())
	{
		LoadMesh(&XFile, &FileName[0]);

		//@ƒNƒ[ƒ“ì¬
		LPD3DXMESH tmpMesh;
		XFile.lpMesh->CloneMeshFVF(D3DXMESH_NPATCHES | D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, lpD3DDevice, &tmpMesh);
		XFile.lpMesh->Release();

		XFile.lpMesh = tmpMesh;

		MeshList[FileName] = XFile;
	}
	else
	{
		XFile = MeshList[FileName];
	}
	return XFile;
}





void MeshManager::LoadMesh(struct XFILE* XFile, const char FName[])
{
	LPD3DXBUFFER lpD3DXBuffer;

	D3DXLoadMeshFromX(FName, D3DXMESH_MANAGED, lpD3DDevice, NULL, &lpD3DXBuffer, NULL, &(XFile->NumMaterial), &(XFile->lpMesh));

	XFile->Mat = new D3DMATERIAL9[XFile->NumMaterial];
	XFile->Tex = new LPDIRECT3DTEXTURE9[XFile->NumMaterial];

	D3DXMATERIAL* D3DXMat = (D3DXMATERIAL*)lpD3DXBuffer->GetBufferPointer();

	DWORD i;
	for (i = 0; i < XFile->NumMaterial; i++)
	{
		XFile->Mat[i] = D3DXMat[i].MatD3D;
		XFile->Mat[i].Ambient = XFile->Mat[i].Diffuse;
		if (FAILED(D3DXCreateTextureFromFile(lpD3DDevice, D3DXMat[i].pTextureFilename, &(XFile->Tex[i]))))
		{
			XFile->Tex[i] = NULL;
		}
	}

	lpD3DXBuffer->Release();
}

void MeshManager::ReleaseMesh(struct XFILE *XFile)
{
	if (XFile->lpMesh != NULL)
	{
		DWORD i;
		delete[] XFile->Mat;

		for (i = 0; i < XFile->NumMaterial; i++)
		{
			if (XFile->Tex[i] != NULL)
			{
				XFile->Tex[i]->Release();
				XFile->Tex[i] = NULL;
			}
		}
		delete[] XFile->Tex;
		XFile->lpMesh->Release();

		XFile->lpMesh = NULL;
	}
}

void MeshManager::AllDelete()
{
	for (auto itr = MeshList.begin(); itr != MeshList.end(); ++itr)
	{
		ReleaseMesh(&itr->second);
	}

	MeshList.clear();
}
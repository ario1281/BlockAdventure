#include"../header/StageBase.h"

extern MeshManager	meshManager;

StageBase::StageBase()
{

	Mesh = meshManager.GetMesh("data/mat/mesh/stage.x");
	D3DXMatrixScaling(&ScaleMat, 200, 200, 200);
	D3DXMatrixTranslation(&TransMat, 0, 0, 0);
	D3DXMatrixIdentity(&RotMat);
	Mat = ScaleMat * TransMat;
}
StageBase::~StageBase()
{
}

bool StageBase::Moving()
{
	float Ang = 0.01f;
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(Ang));

	Mat = RotMat * Mat;
	return true;
}

void StageBase::Render3D()
{
	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);
}
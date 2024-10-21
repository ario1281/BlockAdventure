#include"../header/ObjectWood.h"

ObjectWood::ObjectWood(const D3DXVECTOR3* sPos, char ID, int NumID)
{
	objID = ID;
	objNo = NumID;

	Mesh = meshManager.GetMesh("data/mat/mesh/wood.x");

	Pos = *sPos;
	D3DXMatrixTranslation(&TransMat, Pos.x, Pos.y, Pos.z);
	D3DXMatrixIdentity(&RotMat);

	Mat = RotMat * TransMat;
}
ObjectWood::~ObjectWood()
{
}

void ObjectWood::Render3D()
{
	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);
}
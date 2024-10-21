#include<vector>
#include"../header/ObjectStone.h"

ObjectStone::ObjectStone(const D3DXVECTOR3* sPos, char ID, int NumID)
{
	objID = ID;
	objNo = NumID;

	Mesh = meshManager.GetMesh("data/mat/mesh/stone.x");

	Fall = 0.0f;

	D3DXMatrixTranslation(&TransMat, sPos->x, sPos->y, sPos->z);
	D3DXMatrixIdentity(&RotMat);

	Update();
}
ObjectStone::~ObjectStone()
{

}

void ObjectStone::Update()
{
	Mat = RotMat * TransMat;
}


bool ObjectStone::Falling()
{
	Fall += axelFall;
	if (Fall > maxFall) Fall = maxFall;

	TransMat._42 -= Fall;
	return true;
}



void ObjectStone::Render3D()
{
	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);
}
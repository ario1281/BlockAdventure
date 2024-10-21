#include"../header/ObjectBack.h"

//extern MeshManager	meshManager;

ObjectBack::ObjectBack()
{
	Mesh = meshManager.GetMesh("data/mat/mesh/bg_sphere.x");
	D3DXMatrixScaling(&ScaleMat, 200, 200, 200);
	D3DXMatrixTranslation(&TransMat, 0, 0, 0);
	D3DXMatrixIdentity(&RotMat);
	Mat = ScaleMat * TransMat;
}
ObjectBack::~ObjectBack()
{

}



bool ObjectBack::Moving()
{
	float Ang = 0.01f;
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(Ang));
	
	Mat = RotMat * Mat;
	return true;
}

void ObjectBack::Render3D()
{
	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);
}
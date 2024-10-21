#include"../header/ObjectStage.h"

ObjectStage::ObjectStage()
{
	Mesh = meshManager.GetMesh("data/mat/mesh/stage.x");
	D3DXMatrixScaling(&ScaleMat, 200, 200, 200);
	D3DXMatrixTranslation(&TransMat, 0, 0, 0);
	D3DXMatrixIdentity(&RotMat);
	Mat = ScaleMat * RotMat * TransMat;
}
ObjectStage::~ObjectStage()
{
}

void ObjectStage::Render3D()
{
	lpD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);

	lpD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS,FALSE);

}
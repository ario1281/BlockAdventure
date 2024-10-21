#include<vector>
#include"../header/ObjectSwitch.h"

ObjectSwitch::ObjectSwitch(const D3DXVECTOR3* sPos, char ID, int NumID, int dirID_1, int dirID_2)
{
	objID = ID;
	objNo = NumID;

	Mesh = meshManager.GetMesh("data/mat/mesh/switch.x");
	sndManager.GetSound(rotSnd, rot3DSnd, "data/mat/snd/stage_rotation.wav");

	if (*taskManager.SEGetter() != 0)	Vol = (int)(log10f((float)*taskManager.SEGetter()) * 5000.0f) - 10000;
	else								Vol = -10000;
	rotSnd->SetVolume(Vol);

#pragma region obj方向 (矢印)

	switch (dirID_2)
	{
	case 0:
		D3DXMatrixRotationY(&RotMat, D3DXToRadian(ROTATION_ANG * 0));
		break;

	case 1:
		D3DXMatrixRotationY(&RotMat, D3DXToRadian(ROTATION_ANG * 2));
		break;

	case 2:
		D3DXMatrixRotationY(&RotMat, D3DXToRadian(ROTATION_ANG * 3));
		break;

	case 3:
		D3DXMatrixRotationY(&RotMat, D3DXToRadian(ROTATION_ANG * 1));
		break;
	}
#pragma endregion

#pragma region obj方向(床の配置位置)
	D3DXMATRIX tmpMat;
	switch (dirID_1)
	{
	case down:
		D3DXMatrixRotationZ(&tmpMat, D3DXToRadian(0));
		break;

	case left:
		D3DXMatrixRotationZ(&tmpMat, D3DXToRadian(-90));
		break;

	case rear:
		D3DXMatrixRotationX(&tmpMat, D3DXToRadian(90));
		break;

	case front:
		D3DXMatrixRotationX(&tmpMat, D3DXToRadian(-90));
		break;

	case right:
		D3DXMatrixRotationZ(&tmpMat, D3DXToRadian(90));
		break;

	case up:
		D3DXMatrixRotationX(&tmpMat, D3DXToRadian(180));
		break;

	default:
		D3DXMatrixRotationZ(&tmpMat, D3DXToRadian(0));
		break;
	}
#pragma endregion

	RotMat *= tmpMat;

	//　方向ベクトル
	D3DXMatrixTranslation(&TransMat, sPos->x, sPos->y, sPos->z);
	D3DXVec3TransformNormal(&axisVec, &D3DXVECTOR3(1, 0, 0), &RotMat);
	Mat = RotMat * TransMat;

	Flg = false;
}
ObjectSwitch::~ObjectSwitch()
{
	rotSnd->Release();
	rot3DSnd->Release();
}




bool ObjectSwitch::PreparateRotation()
{
	Flg = true;
	stgAng = ROTATION_ANG;
	return true;
}
bool ObjectSwitch::StageRotation()
{
	float tmpAng;
	if (stgAng >= LITTLE_ROT_ANG)
	{
		tmpAng = LITTLE_ROT_ANG;
	}
	if (stgAng < LITTLE_ROT_ANG)
	{
		rotSnd->SetCurrentPosition(0);
		rotSnd->Stop();
		tmpAng = stgAng;
		Flg = false;
	}

	////　　　回転
	rotSnd->Play(0, 0, 0);
	D3DXMatrixRotationAxis(&stgRotMat, &axisVec, D3DXToRadian(tmpAng));

	stgAng -= tmpAng;

	return true;
}

bool ObjectSwitch::AddToRotation(D3DXVECTOR3 *s_axisVec, D3DXMATRIX *s_RotMat)
{
	if (s_RotMat == nullptr) return false;
	*s_RotMat *= stgRotMat;

	if (s_axisVec != nullptr) D3DXVec3TransformNormal(s_axisVec, s_axisVec, &stgRotMat);

	return true;
}

void ObjectSwitch::Render3D()
{
	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);
}
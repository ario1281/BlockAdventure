#include<vector>
#include"../header/Human.h"

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr) return;
	lpAdd->Release();
	lpAdd = nullptr;
}

Human::Human(const D3DXVECTOR3 *sPos)
{
	Mesh = meshManager.GetMesh("data/mat/mesh/player.x");
	h_Mesh = meshManager.GetMesh("data/mat/mesh/hand.x");

	if (rand() % 100 / 50 < 50)	h_rand = 1;
	else						h_rand = -1;

	Fall = 0.0f;
	Flg = true;
	h_Flg = false;

	Ang = 0.0f;
	D3DXMatrixTranslation(&TransMat, sPos->x, sPos->y, sPos->z);
	D3DXMatrixTranslation(&h_TransMat, 0.48f, 0.38f, 0);
	D3DXMatrixIdentity(&RotMat);
	h_Ang = 0;
	Update();

	p_dirVec = D3DXVECTOR3(0, 0, 0);


}
Human::~Human()
{

}

void Human::Update()
{
	Mat = RotMat * TransMat;

	h_TransMat._41 *= -1;
	h_TransMat._43 *= -1;
	l_Mat = h_TransMat * Mat;
	h_TransMat._41 *= -1;
	h_TransMat._43 *= -1;
	r_Mat = h_TransMat * Mat;
}

bool Human::Moving(const D3DXMATRIX *s_RotMat)
{
	if (s_RotMat == nullptr) return false;

	if (Flg)
	{
#pragma region ˆÚ“®ˆ—
		bool movFlg = false;
		D3DXVECTOR3 movVec(0, 0, 0);
		if (GetAsyncKeyState('W') & 0x8000)
		{
			GetMoveVec(&movVec, &D3DXVECTOR3(0, 0, 1), s_RotMat);
			movFlg = true;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			GetMoveVec(&movVec, &D3DXVECTOR3(0, 0, -1), s_RotMat);
			movFlg = true;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			GetMoveVec(&movVec, &D3DXVECTOR3(-1, 0, 0), s_RotMat);
			movFlg = true;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			GetMoveVec(&movVec, &D3DXVECTOR3(1, 0, 0), s_RotMat);
			movFlg = true;
		}
#pragma endregion

#pragma region UŒüˆ—

		D3DXVec3Normalize(&movVec, &movVec);
		dirVec = movVec;

		if (movFlg == true)
		{
			D3DXMATRIX tmpMat;
			//@ˆÚ“®‚Ì‘¬‚³
#ifdef _DEBUG

			const float Speed = 0.025f;

#else

			const float Speed = 0.025f;

#endif // _DEBUG

			movVec *= Speed;

			D3DXMatrixTranslation(&tmpMat, movVec.x, movVec.y, movVec.z);
			//@ˆÚ“®ˆ—
			TransMat = tmpMat * TransMat;

			D3DXVECTOR3 nowVec;
			D3DXVec3TransformNormal(&nowVec, &D3DXVECTOR3(0, 0, 1), &RotMat);
			D3DXVec3Normalize(&movVec, &movVec);

			float tmpAng;
			tmpAng = D3DXVec3Dot(&nowVec, &movVec);
			
			if (tmpAng > 1) tmpAng = 1;
			if (tmpAng < -1) tmpAng = -1;

			tmpAng = (float)D3DXToDegree(acos(tmpAng));

			D3DXVECTOR3 crsVec;
			D3DXVec3Cross(&crsVec, &nowVec, &movVec);
			D3DXVec3Normalize(&crsVec, &crsVec);

			if (tmpAng >= 5.0f) tmpAng = 5.0f;

			if		(crsVec.y < 0.0f)		Ang -= tmpAng;
			else							Ang += tmpAng;

			D3DXMatrixRotationY(&RotMat, D3DXToRadian(Ang));

			if (h_Flg == false)
			{
				if (rand() % 100 < 50)	h_rand = 1;
				else					h_rand = -1;
			}

			h_Flg = true;

#ifdef _DEBUG
			const float r_wid = 1.75f;
#else
			const float r_wid = 1.25f;
#endif // _DEBUG

			h_TransMat._43 = 0.375f * sin(D3DXToRadian(h_Ang * r_wid));

			h_TransMat._43 *= h_rand;
			h_Ang += 1.5f;
		}
		else
		{
			if (h_Flg == true)
			{
				D3DXMatrixTranslation(&h_TransMat, 0.48f, 0.38f, 0);
				h_Ang = 0;
			}
			h_TransMat._41 = 0.5f + 0.375f * cos(D3DXToRadian(h_Ang * 2.0f))/ 8.0f;
			h_Ang -= 0.5f;
			h_Flg = false;
		}
#pragma endregion
	}
	return true;
}
bool Human::Falling()
{
	Fall += axelFall;
	if (Fall > maxFall) Fall = maxFall;

	TransMat._42 -= Fall;
	return true;
}

void Human::GetMoveVec(D3DXVECTOR3* g_Vec, const D3DXVECTOR3* dirVec, const D3DXMATRIX* YowRotMat)
{
	D3DXVECTOR3 tmpVec;
	D3DXVec3TransformNormal(&tmpVec, dirVec, YowRotMat);
	*g_Vec += tmpVec;
}

void Human::Render3D()
{
	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);

	lpD3DDevice->SetTransform(D3DTS_WORLD, &l_Mat);
	DrawMesh(&h_Mesh);

	lpD3DDevice->SetTransform(D3DTS_WORLD, &r_Mat);
	DrawMesh(&h_Mesh);
}
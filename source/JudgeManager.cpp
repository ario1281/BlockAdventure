#include<vector>

#include"../header/JudgeManager.h"

JudgeManager::JudgeManager()
{

}
JudgeManager::~JudgeManager()
{

}

void JudgeManager::JudgeRay(const LPD3DXMESH lpTarMesh, const D3DXMATRIX *tarMat, const D3DXVECTOR3 *hitPos, const D3DXVECTOR3 *dirVec, BOOL *Hit, DWORD *polyNo, float *mDis)
{
	D3DXMATRIX InvMat;
	D3DXMatrixInverse(&InvMat, NULL, tarMat);

	D3DXVECTOR3 localPos, localVec;
	D3DXVec3TransformCoord(&localPos, hitPos, &InvMat);
	D3DXVec3TransformNormal(&localVec, dirVec, &InvMat);

	D3DXIntersect(lpTarMesh, &localPos, &localVec, Hit, polyNo, NULL, NULL, mDis, NULL, NULL);
}
//void JudgeManager::JudgeSphere(float* gLen, D3DXVECTOR3* gVec, const D3DXVECTOR3* tarPos, const D3DXVECTOR3* stdPos)
//{
//	*gVec = *tarPos - *stdPos;
//
//	// 二点間 の 距離 と 単位ベクトル
//	*gLen = D3DXVec3Length(gVec);
//
//	D3DXVec3Normalize(gVec, gVec);
//}

void JudgeManager::JudgeRayYaw(const D3DXMATRIX *t_hitMat, const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, const float* offset, BOOL* Hit, float* mDis, float jdgLen, const char* Coord)
{
	//　宣言
	D3DXVECTOR3
		hitPos(t_hitMat->_41, t_hitMat->_42, t_hitMat->_43),
		tarPos(tarMat->_41, tarMat->_42, tarMat->_43);

	//　軽量化(レイ判定を抑える
	D3DXVECTOR3 tmpVec;
	float tmpLen;
	tmpVec = hitPos - tarPos;
	tmpLen = D3DXVec3Length(&tmpVec);

	if (tmpLen < jdgLen)
	{
		if (offset != nullptr) hitPos.y += *offset;
		if (Coord == "y")
		{
			JudgeRay(lpTarMesh, tarMat, &hitPos, &D3DXVECTOR3(0, 1, 0), Hit, nullptr, mDis);
		}
		else if (Coord == "my")
		{
			JudgeRay(lpTarMesh, tarMat, &hitPos, &D3DXVECTOR3(0, -1, 0), Hit, nullptr, mDis);
		}
	}
}
void JudgeManager::JudgeRayPitchRoll(D3DXMATRIX* hitMat, const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, float jdgLen)
{
	//　宣言
	D3DXVECTOR3
		hitPos(hitMat->_41, hitMat->_42, hitMat->_43),
		tarPos(tarMat->_41, tarMat->_42, tarMat->_43);

	//　軽量化(レイ判定を抑える
	D3DXVECTOR3 lenVec;
	float tmpLen;
	lenVec = hitPos - tarPos;
	tmpLen = D3DXVec3Length(&lenVec);

	if (tmpLen < jdgLen)
	{
		GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(1, 0, 0), hitMat, 0.5f, "x");
		GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(-1, 0, 0), hitMat, 0.5f, "mx");
		GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(0, 0, 1), hitMat, 0.5f, "z");
		GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(0, 0, -1), hitMat, 0.5f, "mz");
	}
}



bool JudgeManager::GetPos(const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, const D3DXVECTOR3* dirVec, D3DXMATRIX *t_hitMat, float r_Len, const char* Coord)
{
	D3DXVECTOR3 hitPos(t_hitMat->_41, t_hitMat->_42, t_hitMat->_43);
	BOOL Hit;
	float mDis;

	JudgeRay(lpTarMesh, tarMat, &hitPos, dirVec, &Hit, nullptr, &mDis);
	if (Hit == TRUE)
	{
		if (mDis < r_Len)
		{
			if		(Coord == "x")	hitPos.x -= r_Len - mDis;
			else if (Coord == "mx")	hitPos.x += r_Len - mDis;
			else if (Coord == "z")	hitPos.z -= r_Len - mDis;
			else if (Coord == "mz")	hitPos.z += r_Len - mDis;

			D3DXMatrixTranslation(t_hitMat, hitPos.x, hitPos.y, hitPos.z);
		}
		return true;
	}
	return false;
}

bool JudgeManager::JudgeRayPitchRollArrow(D3DXMATRIX* hitMat, const D3DXVECTOR3 *dirVec, 
						const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, float jdgLen)
{
	//　宣言
	D3DXVECTOR3
		hitPos(hitMat->_41, hitMat->_42, hitMat->_43),
		tarPos(tarMat->_41, tarMat->_42, tarMat->_43);

	//　軽量化(レイ判定を抑える
	D3DXVECTOR3 lenVec;
	float tmpLen;
	lenVec = hitPos - tarPos;
	tmpLen = D3DXVec3Length(&lenVec);

	if (tmpLen >= jdgLen) return false;
	
	float tmpAng;
	tmpAng = D3DXVec3Dot(&D3DXVECTOR3(0, 0, 1), dirVec);

	if (tmpAng >= 1) tmpAng = 1;
	if (tmpAng <= -1) tmpAng = -1;

	tmpAng = (float)D3DXToDegree(acos(tmpAng));

	if (tmpAng > 180)	tmpAng -= 360;
	if (tmpAng <= -180) tmpAng += 360;

	D3DXVECTOR3 crsVec;
	D3DXVec3Cross(&crsVec, &D3DXVECTOR3(0, 0, 1), dirVec);
	D3DXVec3Normalize(&crsVec, &crsVec);

	if (crsVec.y < 0.0f)	tmpAng *= -1;

	bool  r_Flg = false;
	if (0 < tmpAng && tmpAng < 180)
	{
		if (GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(1, 0, 0), hitMat, 0.5f, "x")) r_Flg = true;
	}
	if (0 > tmpAng && tmpAng > -180)
	{
		if (GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(-1, 0, 0), hitMat, 0.5f, "mx")) r_Flg = true;
	}
	if (90 > tmpAng && tmpAng >= 0 || 0 > tmpAng && tmpAng > -90)
	{
		if (GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(0, 0, 1), hitMat, 0.5f, "z")) r_Flg = true;
	}
	if (90 < tmpAng && tmpAng <= 180 || -180 < tmpAng && tmpAng < -90)
	{
		if (GetPos(lpTarMesh, tarMat, &D3DXVECTOR3(0, 0, -1), hitMat, 0.5f, "mz")) r_Flg = true;
	}
	return r_Flg;
}

void JudgeManager::JudgeMesh(D3DXMATRIX* hitMat, LPD3DXMESH tarMesh, const D3DXMATRIX* tarMat, const D3DXVECTOR3* dirVec, float jdgLen)
{
	D3DXVECTOR3 hitPos(hitMat->_41, hitMat->_42, hitMat->_43);
	DWORD polyNo;
	BOOL Hit;
	float mDis;

	JudgeRay(tarMesh, tarMat, &hitPos, dirVec, &Hit, &polyNo, &mDis);
	if (Hit)
	{
		if (mDis < jdgLen)
		{
			WORD *pI;
			tarMesh->LockIndexBuffer(0, (LPVOID*)&pI);

			DWORD VerNo[3];
			VerNo[0] = *(pI + polyNo * 3 + 0);
			VerNo[1] = *(pI + polyNo * 3 + 1);
			VerNo[2] = *(pI + polyNo * 3 + 2);

			tarMesh->UnlockIndexBuffer();

			CLONEVERTEX *pV;
			tarMesh->LockVertexBuffer(0, (LPVOID *)&pV);

			D3DXVECTOR3 VPos[3];
			VPos[0] = (pV + VerNo[0])->Pos;
			VPos[1] = (pV + VerNo[1])->Pos;
			VPos[2] = (pV + VerNo[2])->Pos;

			tarMesh->UnlockVertexBuffer();

			D3DXVECTOR3 Vec1, Vec2;
			Vec1 = VPos[1] - VPos[0];
			Vec2 = VPos[2] - VPos[0];

			D3DXVECTOR3 tarVec;
			D3DXVec3Cross(&tarVec, &Vec1, &Vec2);

			D3DXVec3TransformNormal(&tarVec, &tarVec, tarMat);

			D3DXVec3Normalize(&tarVec, &tarVec);

			float Dot;
			Dot = D3DXVec3Dot(&tarVec, &-(*dirVec));

			float Limit;
			Limit = jdgLen / Dot;

			if (Limit < 0) Limit *= 1;
			if (mDis < Limit) hitPos += tarVec * ((Limit - mDis) * Dot);

//			hitPos += *dirVec;

			D3DXMatrixTranslation(hitMat, hitPos.x, hitPos.y, hitPos.z);
		}
	}

	

}
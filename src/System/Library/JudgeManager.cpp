#include "Pch.h"
#include "JudgeManager.h"

void JudgeManager::TransformInverse(const Matrix& s_mInv, Vector3& s_vPos, Vector3& s_vDir)
{
	Matrix	mInv = s_mInv;
	mInv.Inverse();

	s_vPos.TransCoord(mInv);
	s_vDir.TransNormal(mInv);
}

void JudgeManager::JudgingLayToMesh(const LPD3DXMESH& lpMesh, const Matrix& s_mTar, const Vector3& s_vPos, const Vector3& s_vDir, bool *pHit, DWORD *pPolyNo, float *pDist, DWORD *pHitCnt)
{
	Matrix	mInv = s_mTar;
	Vector3 vPos = s_vPos;
	Vector3 vDir = s_vDir;
	TransformInverse(mInv, vPos, vDir);

	BOOL hit;
	D3DXIntersect(
		lpMesh, &vPos, &vDir, &hit, pPolyNo, NULL, NULL, pDist, NULL, pHitCnt);

	hit ? *pHit = true : *pHit = false;
}

bool JudgeManager::JudgeSphere(const Vector3& vTarget, const Vector3& vBase, float disLen)
{
	Vector3 vDist = vTarget - vBase;
	return (vDist.Length() < disLen) ? true : false;
}

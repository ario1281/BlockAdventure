#pragma once
#include"define.h"

class JudgeManager
{
public:
	JudgeManager();
	~JudgeManager();

	static void JudgeRay(const LPD3DXMESH lpTarMesh, const D3DXMATRIX *tarMat, const D3DXVECTOR3 *hitPos, const D3DXVECTOR3 *dirVec, BOOL *Hit, DWORD *polyNo, float *mDis);
	static void JudgeSphere(float* gLen, D3DXVECTOR3* gVec, const D3DXVECTOR3* tarPos, const D3DXVECTOR3* stdPos);

	static void JudgeRayYaw(const D3DXMATRIX *t_hitMat, const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, const float* offset, BOOL* Hit, float* mDis, float jdgLen, const char* Coord);
	static void JudgeRayPitchRoll(D3DXMATRIX* t_hitMat, const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, float jdgLen);
	static bool JudgeRayPitchRollArrow(D3DXMATRIX* t_hitMat, const D3DXVECTOR3 *dirVec, const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, float jdgLen);

	static void JudgeMesh(D3DXMATRIX* hitMat, LPD3DXMESH tarMesh, const D3DXMATRIX* tarMat, const D3DXVECTOR3* dirVec, float jdgLen);

private:
	static bool GetPos(const LPD3DXMESH lpTarMesh, const D3DXMATRIX* tarMat, const D3DXVECTOR3* dirVec, D3DXMATRIX *t_hitMat, float r_Len, const char* Coord);

	bool swchFlg;
	bool glsFlg;
	float sFall;
};
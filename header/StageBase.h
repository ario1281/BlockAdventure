#pragma once
#include<d3dx9.h>

#include"../header/define.h"
#include"../header/MeshManager.h"

class StageBase
{
public:
	StageBase();
	~StageBase();
	
	bool Moving();
	void Render3D();

private:
	XFILE Mesh;
	D3DXMATRIX ScaleMat;
	D3DXMATRIX Mat, TransMat, RotMat;
};
#pragma once
#include"define.h"

class ObjectBack
{
public:
	ObjectBack();
	~ObjectBack();

	bool Moving();
	void Render3D();

private:
	XFILE Mesh;
	D3DXMATRIX Mat, ScaleMat, TransMat, RotMat;
};
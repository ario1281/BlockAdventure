#pragma once
#include"define.h"

class ObjectStage
{
public:
	ObjectStage();
	~ObjectStage();

	void Render3D();

	inline const LPD3DXMESH MeshGetter() { return Mesh.lpMesh; };
	inline const D3DXMATRIX* MatGetter() { return &Mat; };

	inline void MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat);

private:
	XFILE Mesh;
	D3DXMATRIX ScaleMat;
	D3DXMATRIX Mat, TransMat, RotMat;
};

void ObjectStage::MatSetter(const D3DXMATRIX* s_Mat, const D3DXMATRIX* s_RotMat, const D3DXMATRIX* s_TransMat)
{
	if (s_Mat != nullptr)		Mat = *s_Mat;
	if (s_RotMat != nullptr)	RotMat = *s_RotMat;
	if (s_TransMat != nullptr)	TransMat = *s_TransMat;
}
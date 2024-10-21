#pragma once
#include"define.h"

class Cursor
{
public:
	Cursor();
	~Cursor();
	bool Rotation();

	//　アクセサー
	inline const D3DXMATRIX* RotXMatSetter() { return &RotXMat; };
	inline const D3DXMATRIX* RotYMatSetter() { return &RotYMat; };

	//inline const D3DXMATRIX* absRotXMatSetter();
	inline const D3DXMATRIX* absRotYMatSetter();

private:
	POINT BasePt;
	int op_X, op_Y;

	//　カメラ
	float AngX, AngY;
	float a_AngX, a_AngY;
	D3DXMATRIX RotXMat, RotYMat,
		absRotYMat;
};

const D3DXMATRIX* Cursor::absRotYMatSetter()
{
	D3DXMatrixRotationY(&absRotYMat, D3DXToRadian(a_AngY));
	return &absRotYMat;
}
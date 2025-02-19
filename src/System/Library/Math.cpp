#include"define.h"
#include"Math.h"

void Vector3::TransNormal(const Matrix & m) {
	D3DXVec3TransformNormal(this, this, &m);
}

void Vector3::TransCoord(const Matrix & m) {
	D3DXVec3TransformCoord(this, this, &m);
}

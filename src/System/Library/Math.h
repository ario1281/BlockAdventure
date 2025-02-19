#pragma once

class Matrix;

//�@Vector3���b�v�N���X
class Vector3 : public D3DXVECTOR3
{
public:
	//�@�R���X�g���N�^
	Vector3() {
		x = y = z = 0;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	
	//�@�R�s�[�R���X�g���N�^
	Vector3(const D3DXVECTOR3 &_v) {
		*(D3DXVECTOR3*)this = _v;
	}
	Vector3& operator=(const D3DXVECTOR3& _v) {
		*(D3DXVECTOR3*)this = _v;
		return *this;
	}

	void Set(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	void Normalize() {
		D3DXVec3Normalize(this, this);
	}
	void SetLength(float len) {
		Normalize();
		*this *= len;
	}

	float Length() const {
		return D3DXVec3Length(this);
	}

	void TransNormal(const Matrix& m);
	void TransCoord(const Matrix& m);
};

//�@Matrix���b�v�N���X
class Matrix : public D3DXMATRIX {
public:
	//�@�R���X�g���N�^
	Matrix() {
		D3DXMatrixIdentity(this);
	}
	//�@�R�s�[�R���X�g���N�^
	Matrix(const D3DXMATRIX& m) {
		*(D3DXMATRIX*)this = m;
	}
	Matrix& operator=(const D3DXMATRIX& m) {
		*(D3DXMATRIX*)this = m;
		return *this;
	}

	//===========================
	// �u�쐬�v�n
	//===========================
	void CreateIdentity() {
		D3DXMatrixIdentity(this);
	}

	void CreateTranslation(float x, float y, float z) {
		D3DXMatrixTranslation(this, x, y, z);
	}
	void CreateTranslation(const Vector3& v) {
		D3DXMatrixTranslation(this, v.x, v.y, v.z);
	}

	void SetPos(const Vector3 &v) {
		_41 = v.x;
		_42 = v.y;
		_43 = v.z;
	}
	
	void CreateRotationX(float radian) {
		D3DXMatrixRotationX(this, radian);
	}
	void CreateRotationY(float radian) {
		D3DXMatrixRotationY(this, radian);
	}
	void CreateRotationZ(float radian) {
		D3DXMatrixRotationZ(this, radian);
	}

	void CreateScaling(float x, float y, float z) {
		D3DXMatrixScaling(this, x, y, z);
	}


	//===========================
	// �u����v�n
	//===========================
	void Move(float x, float y, float z) {
		Matrix m;
		m.CreateTranslation(x, y, z);
		*this = (*this) * m;
	}
	void Move(const Vector3& v) {
		Matrix m;
		m.CreateTranslation(v.x, v.y, v.z);
		*this = (*this) * m;
	}
	void MoveLocal(float x, float y, float z) {
		Matrix m;
		m.CreateTranslation(x, y, z);
		*this = m * (*this);
	}
	void MoveLocal(const Vector3& v) {
		Matrix m;
		m.CreateTranslation(v.x, v.y, v.z);
		*this = m * (*this);
	}

	//�@��]�s��̍���
	void RotateX(float radian)	//�@(���[���h�ϊ�)
	{
		Matrix m;
		m.CreateRotationX(radian);
		*this = (*this) * m;
	}
	void RotateXLocal(float radian) { //�@(���[�J���ϊ�)
		Matrix m;
		m.CreateRotationX(radian);
		*this = m * (*this);
	}
	
	void RotateY(float radian) {
		Matrix m;
		m.CreateRotationY(radian);
		*this = (*this) * m;
	}
	void RotateYLocal(float radian) {
		Matrix m;
		m.CreateRotationY(radian);
		*this = m * (*this);
	}
	
	void RotateZ(float radian) {
		Matrix m;
		m.CreateRotationZ(radian);
		*this = (*this) * m;
	}
	void RotateZLocal(float radian) {
		Matrix m;
		m.CreateRotationZ(radian);
		*this = m * (*this);
	}

	void RotateAxis(Vector3& axis, float radian) {
		Matrix m;
		D3DXMatrixRotationAxis(&m, &axis, radian);
		*this = (*this) * m;
	}
	void RotateAxisLocal(Vector3 &axis,float radian) {
		Matrix m;
		D3DXMatrixRotationAxis(&m, &axis, radian);
		*this = m * (*this);
	}

	void Scale(float x, float y, float z) {
		Matrix m;
		m.CreateScaling(x, y, z);
		*this = (*this) * m;
	}
	void ScaleLocal(float x, float y, float z) {
		Matrix m;
		m.CreateScaling(x, y, z);
		*this = m * (*this);
	}
	
	// �@�s������߂�
	void Inverse() {
		D3DXMatrixInverse(this, NULL, this);
	}

	//�@�r���{�[�h����
	void SetBillBoard(const Matrix& mCam) {
		// �����̊g�嗦���擾
		float sX = GetXScale();
		float sY = GetYScale();
		float sZ = GetZScale();
		// �J�����̉�]���R�s�[(�����̊g�嗦���l��)
		_11 = mCam._11 * sX;
		_12 = mCam._12 * sX;
		_13 = mCam._13 * sX;
		_21 = mCam._21 * sY;
		_22 = mCam._22 * sY;
		_23 = mCam._23 * sY;
		_31 = mCam._31 * sZ;
		_32 = mCam._32 * sZ;
		_33 = mCam._33 * sZ;
	}

	//===========================
	// �u�擾�v�n
	//===========================
	const Vector3& GetPos()const {
		return *(Vector3*)&_41;
	}
	const Vector3& GetXAxis()const {
		return *(Vector3*)&_11;
	}
	const Vector3& GetYAxis()const {
		return *(Vector3*)&_21;
	}
	const Vector3& GetZAxis()const {
		return *(Vector3*)&_31;
	}

	// X���g�嗦�擾
	float GetXScale() const {
		return GetXAxis().Length();
	}
	// Y���g�嗦�擾
	float GetYScale() const {
		return GetYAxis().Length();
	}
	// Z���g�嗦�擾
	float GetZScale() const {
		return GetZAxis().Length();
	}
};
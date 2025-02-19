#pragma once

class CameraManager
{
public:
	CameraManager() {}
	
	#pragma region 取得系

	//　カメラ行列の設定
	Matrix&		GetCamMat()			{ return m_mCam; }

	const Matrix& GetView() const	{ return m_mView; }
	const Matrix& GetProj() const	{ return m_mProj; }

	#pragma endregion

	//　カメラ行列 → ビュー行列の変換
	void CameraToView() {
		m_mView = m_mCam;
		m_mView.Inverse();
	}

	//　射影行列の作成
	void CreateParspectiveFovLH(float fov, float aspect, float zNear, float zFar) {
		D3DXMatrixPerspectiveFovLH(&m_mProj, fov, aspect, zNear, zFar);
	}

private:
	Matrix m_mCam;
	Matrix m_mView;
	Matrix m_mProj;
};
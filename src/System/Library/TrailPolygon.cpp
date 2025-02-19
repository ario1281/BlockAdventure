#include"define.h"
#include"TrailPolygon.h"

void TrailPolygon::Draw(float width)
{
	if (m_matList.size() < 2) return;

	float uvCount = float(m_matList.size() - 1);

	struct VERTEX {
		D3DXVECTOR3 pos;	// 座標
		DWORD		color;	// 色
		D3DXVECTOR2	uv;		// UV座標
	};
	std::vector<VERTEX> vertex;
	vertex.resize(m_matList.size() * 2);

	//--------------------------
	// 頂点データ作成
	//--------------------------
	for (UINT mi = 0; mi < m_matList.size(); mi++) {
		//　登録行列 ショートカット
		Matrix &mat = m_matList[mi];

		// ２つの頂点 ショートカット
		VERTEX &v1 = vertex[mi * 2];
		VERTEX &v2 = vertex[mi * 2 + 1];

		Vector3 axisX = mat.GetXAxis();
		axisX.Normalize();

		//　座標
		v1.pos = mat.GetPos() + axisX * width * 0.5f;
		v2.pos = mat.GetPos() + axisX * width * 0.5f;

		//　色
		v1.color = D3DCOLOR_ARGB(255, 255, 255, 255);
		v2.color = D3DCOLOR_ARGB(255, 255, 255, 255);

		//　UV
		v1.uv = { 0, mi / uvCount };
		v2.uv = { 1, mi / uvCount };
	}

	DEVICE->SetFVF(FVF_VERTEX);

	DEVICE->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		(m_matList.size() - 1) * 2,	// 描画する面数
		&vertex[0],					// 頂点の先頭アドレス
		sizeof(VERTEX)				// 1頂点のサイズ
	);
}

void TrailPolygon::DrawBillBoard(float width)
{
	if (m_matList.size() < 2) return;

	Matrix mCam;
	DEVICE->GetTransform(D3DTS_VIEW, &mCam);
	mCam.Inverse();

	float uvCount = float(m_matList.size() - 1);

	struct VERTEX {
		D3DXVECTOR3 pos;	// 座標
		DWORD		color;	// 色
		D3DXVECTOR2	uv;		// UV座標
	};
	std::vector<VERTEX> vertex;
	vertex.resize(m_matList.size() * 2);

	//--------------------------
	// 頂点データ作成
	//--------------------------
	for (UINT mi = 0; mi < m_matList.size(); mi++) {
		//　登録行列 ショートカット
		Matrix &mat = m_matList[mi];

		// ２つの頂点 ショートカット
		VERTEX &v1 = vertex[mi * 2];
		VERTEX &v2 = vertex[mi * 2 + 1];

		Vector3 v = mat.GetPos() - mCam.GetPos();
		Vector3 axisX;
		D3DXVec3Cross(&axisX, &mat.GetZAxis(), &v);
		axisX.Normalize();

		//　座標
		v1.pos = mat.GetPos() + axisX * width * 0.5f;
		v2.pos = mat.GetPos() + axisX * width * 0.5f;

		//　座標
		v1.color = D3DCOLOR_ARGB(255, 255, 255, 255);
		v2.color = D3DCOLOR_ARGB(255, 255, 255, 255);

		//　UV
		v1.uv = { 0, mi / uvCount };
		v2.uv = { 1, mi / uvCount };
	}

	DEVICE->SetFVF(FVF_VERTEX);

	DEVICE->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		(m_matList.size() - 1) * 2,
		&vertex[0],
		sizeof(VERTEX));
}

void TrailPolygon::DrawStrip()
{
	if (m_matList.size() < 3) return;
	struct VERTEX {
		D3DXVECTOR3 pos;	// 座標
		DWORD		color;	// 色
		D3DXVECTOR2	uv;		// UV座標
	};
	std::vector<VERTEX> vertex;
	vertex.resize(m_matList.size());

	int num = m_matList.size() / 2;

	//　頂点データ作成
	for (UINT mi = 0; mi < m_matList.size(); mi++) {
		Matrix &mat = m_matList[mi];
		VERTEX &v1 = vertex[mi];

		v1.pos = mat.GetPos();
		v1.color = D3DCOLOR_ARGB(255, 255, 255, 255);
		v1.uv.x = float(mi % 2);
		v1.uv.y = (num - mi / 2) / (float)num;
	}

	DEVICE->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	DEVICE->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		(m_matList.size() - 1) * 2,
		&vertex[0],
		sizeof(VERTEX));
}

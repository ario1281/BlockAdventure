#include"define.h"
#include"TrailPolygon.h"

void TrailPolygon::Draw(float width)
{
	if (m_matList.size() < 2) return;

	float uvCount = float(m_matList.size() - 1);

	struct VERTEX {
		D3DXVECTOR3 pos;	// ���W
		DWORD		color;	// �F
		D3DXVECTOR2	uv;		// UV���W
	};
	std::vector<VERTEX> vertex;
	vertex.resize(m_matList.size() * 2);

	//--------------------------
	// ���_�f�[�^�쐬
	//--------------------------
	for (UINT mi = 0; mi < m_matList.size(); mi++) {
		//�@�o�^�s�� �V���[�g�J�b�g
		Matrix &mat = m_matList[mi];

		// �Q�̒��_ �V���[�g�J�b�g
		VERTEX &v1 = vertex[mi * 2];
		VERTEX &v2 = vertex[mi * 2 + 1];

		Vector3 axisX = mat.GetXAxis();
		axisX.Normalize();

		//�@���W
		v1.pos = mat.GetPos() + axisX * width * 0.5f;
		v2.pos = mat.GetPos() + axisX * width * 0.5f;

		//�@�F
		v1.color = D3DCOLOR_ARGB(255, 255, 255, 255);
		v2.color = D3DCOLOR_ARGB(255, 255, 255, 255);

		//�@UV
		v1.uv = { 0, mi / uvCount };
		v2.uv = { 1, mi / uvCount };
	}

	DEVICE->SetFVF(FVF_VERTEX);

	DEVICE->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		(m_matList.size() - 1) * 2,	// �`�悷��ʐ�
		&vertex[0],					// ���_�̐擪�A�h���X
		sizeof(VERTEX)				// 1���_�̃T�C�Y
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
		D3DXVECTOR3 pos;	// ���W
		DWORD		color;	// �F
		D3DXVECTOR2	uv;		// UV���W
	};
	std::vector<VERTEX> vertex;
	vertex.resize(m_matList.size() * 2);

	//--------------------------
	// ���_�f�[�^�쐬
	//--------------------------
	for (UINT mi = 0; mi < m_matList.size(); mi++) {
		//�@�o�^�s�� �V���[�g�J�b�g
		Matrix &mat = m_matList[mi];

		// �Q�̒��_ �V���[�g�J�b�g
		VERTEX &v1 = vertex[mi * 2];
		VERTEX &v2 = vertex[mi * 2 + 1];

		Vector3 v = mat.GetPos() - mCam.GetPos();
		Vector3 axisX;
		D3DXVec3Cross(&axisX, &mat.GetZAxis(), &v);
		axisX.Normalize();

		//�@���W
		v1.pos = mat.GetPos() + axisX * width * 0.5f;
		v2.pos = mat.GetPos() + axisX * width * 0.5f;

		//�@���W
		v1.color = D3DCOLOR_ARGB(255, 255, 255, 255);
		v2.color = D3DCOLOR_ARGB(255, 255, 255, 255);

		//�@UV
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
		D3DXVECTOR3 pos;	// ���W
		DWORD		color;	// �F
		D3DXVECTOR2	uv;		// UV���W
	};
	std::vector<VERTEX> vertex;
	vertex.resize(m_matList.size());

	int num = m_matList.size() / 2;

	//�@���_�f�[�^�쐬
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

#include"define.h"
#include"SquarePolygon.h"

void SquarePolygon::Init(float w, float h, DWORD color)
{	// ���_���W
	m_vertex[0].pos = { -w / 2, -h / 2, 0 };
	m_vertex[1].pos = { -w / 2,  h / 2, 0 };
	m_vertex[2].pos = { w / 2, -h / 2, 0 };
	m_vertex[3].pos = { w / 2,  h / 2, 0 };
	// ���_�F
	m_vertex[0].color = color;
	m_vertex[1].color = color;
	m_vertex[2].color = color;
	m_vertex[3].color = color;
	// UV���W
	m_vertex[0].uv = { 0, 1 };
	m_vertex[1].uv = { 0, 0 };
	m_vertex[2].uv = { 1, 1 };
	m_vertex[3].uv = { 1, 0 };
}

void SquarePolygon::Draw()
{
	DEVICE->SetFVF(FVF_VERTEX);

	DEVICE->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		&m_vertex[0],
		sizeof(VERTEX)
	);
}

void SquarePolygon::SetAnimationPos(float no)
{
	// �}�X���W
	int x = (int)no % m_animSplitX;
	int y = (int)no / m_animSplitY;
	// �P�}�X�̕��ƍ���
	float w = 1.0f / m_animSplitX;
	float h = 1.0f / m_animSplitY;

	// UV���W
	m_vertex[0].uv = { x*w, (y + 1)*h };
	m_vertex[1].uv = { x*w, y*h };
	m_vertex[2].uv = { (x + 1)*w, (y + 1)*h };
	m_vertex[3].uv = { (x + 1)*w, y*h };

	// �A�j���[�V�����̈ʒu���X�V
	m_animPos = no;

}

void SquarePolygon::Animation(float speed, bool loop)
{

}

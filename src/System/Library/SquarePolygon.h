#pragma once

class SquarePolygon
{
public:
	// �|���S���ݒ�
	// �Ew		�c ��
	// �Eh		�c ����
	// �Ecolor	�c �F
	void Init(float w, float h, DWORD color);
	void Draw();

	void SetAnimationInfo(int splitX, int splitY) {
		m_animSplitX = splitX;
		m_animSplitY = splitY;
	}

	// �����ݒ�����ƂɁA�A�j���[�V�����ʒu�ݒ�
	//�@no	�c�摜�ԍ�
	void SetAnimationPos(float no);

	//	�A�j���[�V�����̐i�s
	//	�Espeed		�c �i�s���x 1.0�łP�R�}����
	//	�Eloop		�c ���[�v�Đ�����H
	void Animation(float speed, bool loop);

	//�@�A�j���[�V�����̏I������
	bool IsAnimationEnd() {
		//�@����
		if (m_animPos >= (m_animSplitX * m_animSplitY) - 1)
			return true;
		return false;
	}

private:
	struct VERTEX {
		D3DXVECTOR3 pos;
		DWORD		color;
		D3DXVECTOR2	uv;
	};

	VERTEX m_vertex[4];

	int m_animSplitX = 1;
	int m_animSplitY = 1;

	float m_animPos = 0;
};
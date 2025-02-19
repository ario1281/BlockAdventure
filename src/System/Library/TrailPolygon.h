#pragma once

class TrailPolygon
{
public:
	//�@�s��ǉ�
	void AddMatrix(const Matrix &mat) {
		m_matList.push_back(mat);
	}

	//�@�s��̍폜
	void DeleteMatrix_Flont() {
		m_matList.pop_front();
	}
	void DeleteMatrix_Back() {
		m_matList.pop_back();
	}

	//�@���X�g���̎擾
	int GetNum()const {
		return (int)m_matList.size();
	}

	//�@�`��
	//�@�Ewidth		�c �|���S���̕�
	void Draw(float width);
	void DrawBillBoard(float width);

	void DrawStrip();

private:
	std::deque<Matrix> m_matList;
};

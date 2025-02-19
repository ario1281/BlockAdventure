#pragma once

class TrailPolygon
{
public:
	//　行列追加
	void AddMatrix(const Matrix &mat) {
		m_matList.push_back(mat);
	}

	//　行列の削除
	void DeleteMatrix_Flont() {
		m_matList.pop_front();
	}
	void DeleteMatrix_Back() {
		m_matList.pop_back();
	}

	//　リスト数の取得
	int GetNum()const {
		return (int)m_matList.size();
	}

	//　描画
	//　・width		… ポリゴンの幅
	void Draw(float width);
	void DrawBillBoard(float width);

	void DrawStrip();

private:
	std::deque<Matrix> m_matList;
};

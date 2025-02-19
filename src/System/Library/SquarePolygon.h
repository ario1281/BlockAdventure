#pragma once

class SquarePolygon
{
public:
	// ポリゴン設定
	// ・w		… 幅
	// ・h		… 高さ
	// ・color	… 色
	void Init(float w, float h, DWORD color);
	void Draw();

	void SetAnimationInfo(int splitX, int splitY) {
		m_animSplitX = splitX;
		m_animSplitY = splitY;
	}

	// 分割設定をもとに、アニメーション位置設定
	//　no	…画像番号
	void SetAnimationPos(float no);

	//	アニメーションの進行
	//	・speed		… 進行速度 1.0で１コマずつ
	//	・loop		… ループ再生する？
	void Animation(float speed, bool loop);

	//　アニメーションの終了判定
	bool IsAnimationEnd() {
		//　判定
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
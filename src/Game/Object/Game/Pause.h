#pragma once
#include"Game\GameObject.h"

class Pause : public GameObject
{
public:
	Pause();
	~Pause() {}

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override;
	bool MouseCursorControl();

	void Draw2D()override;

	void SetInput(const shared_ptr<InputComponent>& spInput) {
		m_wpInput = spInput;
	}
	void SetFlag(bool *s_Flg) {
		m_msgFlg = s_Flg;
	}

private:
	weak_ptr<InputComponent>		m_wpInput;
	weak_ptr<InputComponent>		m_wpFlg;

	shared_ptr<TextureManager>		m_spTex;
	static const int				m_msgCnt = 3;
	RECT							m_rcMsg[m_msgCnt];
	Matrix							m_mMsg[m_msgCnt];
	bool							m_pickFlg[2] = { false,false };
	bool							*m_msgFlg = nullptr;
	float							m_msgTri = 0.0f;

	//　フェード
	shared_ptr <TextureManager>		m_fadeTex;
	int								m_fadeCnt = 0;
	const int						m_fade = 255 / 2;
};
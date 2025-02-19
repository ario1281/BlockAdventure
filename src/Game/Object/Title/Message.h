#pragma once
#include"Game\GameObject.h"

class Message :public GameObject
{
public:
	Message() {
		InitToTitle();
		InitToEnd();
	}
	~Message() {}

	void InitToTitle();
	void InitToEnd();

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override;
	void Draw2D()override;
	
	void StartProcess(const std::list<shared_ptr<GameObject>>* obj);
	void StartDraw2D();

	//　終了画面
	void EndProcess(const std::list<shared_ptr<GameObject>>* obj);
	void EndDraw2D();

	void SetInput(const shared_ptr<InputComponent>& spInput) {
		m_wpInput = spInput;
	}

private:
	weak_ptr<InputComponent>	m_wpInput;

	float							m_triAng				= 0.0f;
	bool							m_stFlg					= false;
	bool							m_edFlg					= false;
	shared_ptr<DirectSoundManager>	m_spSE;

	//　ゲーム終了画面
	shared_ptr<TextureManager>		m_edMsgTex;
	static const int				m_edMsgCnt				= 3;
	RECT							m_rcEdMsg[m_edMsgCnt];
	Matrix							m_mEdMsg[m_edMsgCnt];
	bool							m_pickFlg[2]			= { false,false };
	float							m_edMsgTri				= 0.0f;

	//　フェード
	shared_ptr <TextureManager>		m_fadeTex;
	int								m_fadeCnt				= 0;
	const int						m_fade					= 255 / 2;
};
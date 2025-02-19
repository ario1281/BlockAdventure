#include"System\Library\define.h"
#include"Message.h"

#include"Game\Scene\GameScene.h"


void Message::InitToTitle()
{
	m_spTex = ResFac.GetTexture("data\\title\\st_msg.png");
	m_spSE	= ResFac.GetSound("data\\se\\se_start.wav");

	m_mWorld.CreateTranslation(
		(SCRW - m_spTex->GetRect().right) / 2.0f,
		3.0f * (SCRH - m_spTex->GetRect().bottom) / 4.0f,
		0.0f);
}
void Message::InitToEnd()
{
	m_edMsgTex = ResFac.GetTexture("data\\title\\end_msg.png");
	m_fadeTex = ResFac.GetTexture("data\\title\\fade.png");

	#pragma region 終了テロップ

	m_rcEdMsg[0] = {
	m_edMsgTex->GetRect().left,
	m_edMsgTex->GetRect().top,
	m_edMsgTex->GetRect().right,
	m_edMsgTex->GetRect().bottom * 1 / 3
	};
	m_rcEdMsg[1] = {
		m_edMsgTex->GetRect().left,
		m_edMsgTex->GetRect().bottom * 2 / 3,
		m_edMsgTex->GetRect().right / 2,
		m_edMsgTex->GetRect().bottom
	};
	m_rcEdMsg[2] = {
		m_edMsgTex->GetRect().right / 2,
		m_edMsgTex->GetRect().bottom * 2 / 3,
		m_edMsgTex->GetRect().right,
		m_edMsgTex->GetRect().bottom
	};

	m_mEdMsg[0].CreateTranslation(
		float((SCRW - m_rcEdMsg[0].right) / 2), SCRH / 4, 0);
	m_mEdMsg[1].CreateTranslation(
		float(SCRW / 2.0f - m_rcEdMsg[1].right), SCRH / 2, 0);
	m_mEdMsg[2].CreateTranslation(SCRW / 2.0f, SCRH / 2, 0);

	#pragma endregion

}


bool Message::Update(const std::list<shared_ptr<GameObject>>* obj)
{
	auto input = m_wpInput.lock();

	//　スタート
	if (!APP.GetMouseFlag())
	{
		if (input->Button(input->Bt_1) == input->DOWN)
		{
			if (!m_edFlg)
			{
				m_spSE->Play();
				m_stFlg = true;
			}
		}
		//　やめる
		else if (input->Button(input->Bt_Esc) == input->DOWN)
		{
			if (!m_stFlg) { m_edFlg = true; }
		}
	}

	if (m_edFlg)
	{
		EndProcess(obj);
	}
	StartProcess(obj);

	m_triAng++;
	return true;
}
void Message::Draw2D()
{
	Matrix mTmp;
	mTmp.CreateTranslation(
		0, sinf(D3DXToRadian(m_triAng * 3.75f)) * 32.0f, 0);
	SPRITE.DrawTex(mTmp * m_mWorld, m_spTex.get());

	StartDraw2D();
	EndDraw2D();
}

#pragma region 開始処理
void Message::StartProcess(const std::list<shared_ptr<GameObject>>* obj)
{
	//　スタートボタンを押したか
	if (!m_stFlg) { return; }

	if (m_fadeCnt > 255)
	{
		APP.SetChangeScene(new GameScene());
		return;
	}
}
void Message::StartDraw2D()
{
	//　スタートボタンを押したか
	if (!m_stFlg) { return; }
	//　フェイド(明)
	Matrix mTmp;
	mTmp.CreateScaling(SCRW, SCRH, 0);
	SPRITE.DrawTex(mTmp, m_fadeTex.get(), nullptr,
		D3DCOLOR_ARGB(m_fadeCnt, 255, 255, 255));

	m_fadeCnt += 7;
}
#pragma endregion

#pragma region 終了処理
void Message::EndProcess(const std::list<shared_ptr<GameObject>>* obj)
{
	auto input = m_wpInput.lock();

	#pragma region マウス操作

	Vector3 offset;
	//　はいの方にカーソルが当たってる
	offset.Set(100.0f, 200.0f, 0);
	if (CURSOR.IsRange(
		m_mEdMsg[1]._41 + offset.x,
		m_mEdMsg[1]._42,
		float(m_rcEdMsg[1].right - m_rcEdMsg[1].left) - offset.y,
		float(m_rcEdMsg[1].bottom - m_rcEdMsg[1].top)))
	{
		if (m_pickFlg[0] == false) { m_pickFlg[0] = true; }
	}
	else if (m_pickFlg[0] == true) { m_pickFlg[0] = false; }

	//　いいえの方にカーソルが当たってる
	offset.Set(62.5f, 130.0f, 0);
	if (CURSOR.IsRange(
		m_mEdMsg[2]._41 + offset.x,
		m_mEdMsg[2]._42,
		float(m_rcEdMsg[2].right - m_rcEdMsg[2].left) - offset.y,
		float(m_rcEdMsg[2].bottom - m_rcEdMsg[2].top)))
	{
		if (m_pickFlg[1] == false) { m_pickFlg[1] = true; }
	}
	else if (m_pickFlg[1] == true) { m_pickFlg[1] = false; }

	#pragma endregion

	if (input->Button(input->Bt_1) == input->UP)
	{
		if(m_pickFlg[0] == true)
		{
			APP.EndGame();
			return;
		}
		if(m_pickFlg[1] == true)
		{
			m_edFlg = false;
			m_fadeCnt = 0;
		}
	}

	if (m_pickFlg[0] || m_pickFlg[1]) { m_edMsgTri--; }
	else { m_edMsgTri = 0.0f; }
}
void Message::EndDraw2D()
{
	if (!m_edFlg) { return; }

	//　フェイド(暗)
	{
		Matrix mTmp;
		mTmp.CreateScaling(SCRW, SCRH, 0);
		SPRITE.DrawTex(mTmp, m_fadeTex.get(), nullptr,
			D3DCOLOR_ARGB(m_fadeCnt, 0, 0, 0));

		//　フェイドの制御(アルファ値の制限)
		m_fadeCnt < m_fade ? m_fadeCnt++ : m_fadeCnt = m_fade;
	}

	//　終了画面
	Matrix mShadow;
	mShadow.CreateTranslation(4, 4, 0);
	//　確認のテロップ
	SPRITE.DrawTex(m_mEdMsg[0] * mShadow,
		m_edMsgTex.get(), &m_rcEdMsg[0], D3DCOLOR_ARGB(50, 0, 0, 0));
	SPRITE.DrawTex(m_mEdMsg[0], m_edMsgTex.get(), &m_rcEdMsg[0]);

	Matrix mTmp;
	mTmp.CreateTranslation(
		0, sinf(D3DXToRadian(m_edMsgTri * 3.75f)) * 32.0f, 0);
	//　はいの描画
	if (m_pickFlg[0])
	{
		SPRITE.DrawTex(mTmp * m_mEdMsg[1] * mShadow,
			m_edMsgTex.get(), &m_rcEdMsg[1],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			mTmp * m_mEdMsg[1], m_edMsgTex.get(), &m_rcEdMsg[1]);
	}
	else {
		SPRITE.DrawTex(m_mEdMsg[1] * mShadow,
			m_edMsgTex.get(), &m_rcEdMsg[1],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			m_mEdMsg[1], m_edMsgTex.get(), &m_rcEdMsg[1]);
	}
	//　いいえの描画
	if (m_pickFlg[1])
	{
		SPRITE.DrawTex(mTmp * m_mEdMsg[2] * mShadow,
			m_edMsgTex.get(), &m_rcEdMsg[2],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			mTmp * m_mEdMsg[2], m_edMsgTex.get(), &m_rcEdMsg[2]);
	}
	else {
		SPRITE.DrawTex(m_mEdMsg[2] * mShadow,
			m_edMsgTex.get(), &m_rcEdMsg[2],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			m_mEdMsg[2], m_edMsgTex.get(), &m_rcEdMsg[2]);
	}
}
#pragma endregion

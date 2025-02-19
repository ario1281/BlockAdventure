#include"Pause.h"
#include"Game\Scene\TitleScene.h"

Pause::Pause()
{
	m_spTex = ResFac.GetTexture("data\\title\\end_msg.png");
	m_fadeTex = ResFac.GetTexture("data\\title\\fade.png");

	m_rcMsg[0] = {
		m_spTex->GetRect().left,
		m_spTex->GetRect().bottom * 1 / 3,
		m_spTex->GetRect().right,
		m_spTex->GetRect().bottom * 2 / 3
	};
	m_rcMsg[1] = {
		m_spTex->GetRect().left,
		m_spTex->GetRect().bottom * 2 / 3,
		m_spTex->GetRect().right / 2,
		m_spTex->GetRect().bottom
	};
	m_rcMsg[2] = {
		m_spTex->GetRect().right / 2,
		m_spTex->GetRect().bottom * 2 / 3,
		m_spTex->GetRect().right,
		m_spTex->GetRect().bottom
	};

	m_mMsg[0].CreateTranslation(
		float((SCRW - m_rcMsg[0].right) / 2), SCRH / 4, 0);
	m_mMsg[1].CreateTranslation(
		float(SCRW / 2.0f - m_rcMsg[1].right), SCRH / 2, 0);
	m_mMsg[2].CreateTranslation(SCRW / 2.0f, SCRH / 2, 0);
}

bool Pause::Update(const std::list<shared_ptr<GameObject>>* obj)
{
	auto input = m_wpInput.lock();

	if (input->Button(input->Bt_Esc) == input->DOWN) {
		*m_msgFlg = true;
	}

	if (!m_msgFlg) { return true; }

	MouseCursorControl();

	if (input->Button(input->Bt_1) == input->UP)
	{
		if (m_pickFlg[0] == true)
		{
			APP.SetChangeScene(new TitleScene());
			return true;
		}
		if (m_pickFlg[1] == true)
		{
			*m_msgFlg = false;
			m_fadeCnt = 0;
			CURSOR.SetPos();
		}
	}

	if (m_pickFlg[0] || m_pickFlg[1]) { m_msgTri--; }
	else { m_msgTri = 0.0f; }

	return true;
}
bool Pause::MouseCursorControl()
{
	Vector3 offset;
	//　はいの方にカーソルが当たってる
	offset.Set(100.0f, 200.0f, 0);
	if (CURSOR.IsRange(
		m_mMsg[1]._41 + offset.x,
		m_mMsg[1]._42,
		float(m_rcMsg[1].right - m_rcMsg[1].left) - offset.y,
		float(m_rcMsg[1].bottom - m_rcMsg[1].top)))
	{
		if (m_pickFlg[0] == false) { m_pickFlg[0] = true; }
	}
	else if (m_pickFlg[0] == true) { m_pickFlg[0] = false; }

	//　いいえの方にカーソルが当たってる
	offset.Set(62.5f, 130.0f, 0);
	if (CURSOR.IsRange(
		m_mMsg[2]._41 + offset.x,
		m_mMsg[2]._42,
		float(m_rcMsg[2].right - m_rcMsg[2].left) - offset.y,
		float(m_rcMsg[2].bottom - m_rcMsg[2].top))) {
		if (m_pickFlg[1] == false) { m_pickFlg[1] = true; }
	}
	else if (m_pickFlg[1] == true) { m_pickFlg[1] = false; }

	return true;
}

void Pause::Draw2D()
{
	if (!m_msgFlg) { return; }

	/*　フェイド(暗)　*/ {
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
	SPRITE.DrawTex(m_mMsg[0] * mShadow,
		m_spTex.get(), &m_rcMsg[0], D3DCOLOR_ARGB(50, 0, 0, 0));
	SPRITE.DrawTex(m_mMsg[0], m_spTex.get(), &m_rcMsg[0]);

	Matrix mTmp;
	mTmp.CreateTranslation(
		0, sinf(D3DXToRadian(m_msgTri * 3.75f)) * 32.0f, 0);

	//　はいの描画
	if (m_pickFlg[0])
	{
		SPRITE.DrawTex(mTmp * m_mMsg[1] * mShadow,
			m_spTex.get(), &m_rcMsg[1],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			mTmp * m_mMsg[1], m_spTex.get(), &m_rcMsg[1]);
	}
	else {
		SPRITE.DrawTex(m_mMsg[1] * mShadow,
			m_spTex.get(), &m_rcMsg[1],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			m_mMsg[1], m_spTex.get(), &m_rcMsg[1]);
	}
	//　いいえの描画
	if (m_pickFlg[1])
	{
		SPRITE.DrawTex(mTmp * m_mMsg[2] * mShadow,
			m_spTex.get(), &m_rcMsg[2],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			mTmp * m_mMsg[2], m_spTex.get(), &m_rcMsg[2]);
	}
	else {
		SPRITE.DrawTex(m_mMsg[2] * mShadow,
			m_spTex.get(), &m_rcMsg[2],
			D3DCOLOR_ARGB(50, 0, 0, 0));
		SPRITE.DrawTex(
			m_mMsg[2], m_spTex.get(), &m_rcMsg[2]);
	}
}

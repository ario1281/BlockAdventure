#include"System\Library\define.h"
#include"Logo.h"

TitleLogo::TitleLogo()
{
	m_spTex = ResFac.GetTexture("data\\title\\title_logo.png");
	m_mWorld.CreateTranslation(
		(SCRW - m_spTex->GetRectLength().x) / 2.0f,
		(SCRH - m_spTex->GetRectLength().y) / 4.0f,
		0);
}


void TitleLogo::Draw2D()
{
	SPRITE.DrawTex(m_mWorld, m_spTex.get());
}

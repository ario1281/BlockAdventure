#include"define.h"
#include"InputComponent.h"

void IC_MouseAndKey::Update()
{
	for (int bCnt = 0; bCnt < ButtonNum; bCnt++)
	{
		if (GetAsyncKeyState(m_keyCode[bCnt]))
		{
			Push(bCnt);
		}
		else
		{
			Release(bCnt);
		}
	}
}



void Cursor::Update()
{
	//　マウス操作
	POINT pt;
	GetCursorPos(&pt);
	SetCursorPos(m_pBase.x, m_pBase.y);

	//　カーソルの移動量
	m_vertical -= (m_pBase.x - pt.x) / m_sCtrl;
	m_horizontal -= (m_pBase.y - pt.y) / m_sCtrl;

	if (m_horizontal < -m_rAng) { m_horizontal = -m_rAng; }
	if (m_horizontal > m_rAng) { m_horizontal = m_rAng; }
}
bool Cursor::IsRange(float x, float y, float w, float h)
{
	POINT ptNow, ptScr{ 0,0 };
	GetCursorPos(&ptNow);
	ClientToScreen(APP.GetHWnd(), &ptScr);

	//　レクト判定
	if ((ptNow.x >= ptScr.x + x && ptNow.x <= ptScr.x + x + w) &&
		(ptNow.y >= ptScr.y + y && ptNow.y <= ptScr.y + y + h))
	{
		return true;
	}
	return false;
}

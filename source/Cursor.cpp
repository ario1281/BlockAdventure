#include"../header/Cursor.h"


Cursor::Cursor()
{
	BasePt.x = (LONG)(SCRW / 2.0f);
	BasePt.y = (LONG)(SCRH / 2.0f);

	ClientToScreen(hwnd, &BasePt);

	SetCursorPos(BasePt.x, BasePt.y);
	AngY = 0;
	AngX = 0;

	op_Y = *taskManager.CursorYawGetter();
	op_X = *taskManager.CursorPitchGetter();
}
Cursor::~Cursor()
{

}

bool Cursor::Rotation()
{
	POINT Pt;
	GetCursorPos(&Pt);
	SetCursorPos(BasePt.x, BasePt.y);

	AngY -= (BasePt.x - Pt.x) / 5.0f;
	AngX -= (BasePt.y - Pt.y) / 5.0f;

	AngY -= (BasePt.x - Pt.x) / 5.0f * op_Y;
	AngX -= (BasePt.y - Pt.y) / 5.0f * op_X;

	const float resAng = 60.0f;
	if (AngX < -resAng) AngX = -resAng;
	if (AngX > resAng) AngX = resAng;

	D3DXMatrixRotationY(&RotYMat, D3DXToRadian(AngY));
	D3DXMatrixRotationX(&RotXMat, D3DXToRadian(AngX));
	return true;
}
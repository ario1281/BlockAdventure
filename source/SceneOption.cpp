#include"../header/SceneOption.h"

extern SceneManager		scnManager;

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr) return;
	lpAdd->Release();
	lpAdd = nullptr;
}

SceneOption::SceneOption(const LPDIRECTSOUNDBUFFER8 s_snd, const LPDIRECTSOUND3DBUFFER8 s_3Dsnd)
{
	SetLight();

#pragma region テクスチャ
	slctNum = 0;
	s_TexLen.w = 132;
	s_TexLen.h = 74;
	slctTex = texManager.GetTexture("data/mat/img/select_arrow.png", s_TexLen.w, s_TexLen.h, NULL);

	symLen = TEXW(s_TexLen.w) / 2.0f;

	op_Msg.Len.w = 355;
	op_Msg.Len.h = 105;
	op_Msg.Tex = texManager.GetTexture("data/mat/img/option_message.png", op_Msg.Len.w, op_Msg.Len.h, NULL);
	D3DXMatrixTranslation(&op_Msg.Mat, TEXW(op_Msg.Len.w) / 2.0f, 25, 0);

	float offset = 30.0f;

	op_Mouse.Len.w = 256;
	op_Mouse.Len.h = 64;
	op_Mouse.Tex = texManager.GetTexture("data/mat/img/option_mouse.png", op_Mouse.Len.w, op_Mouse.Len.h, NULL);
	D3DXMatrixTranslation(&op_Mouse.Mat, TEXW(op_Mouse.Len.w) / 2.0f - TEXW(op_Mouse.Len.w) / 6.0f - offset, TEXH(op_Mouse.Len.h) / 3.0f, 0);

	op_1.Len.w = 350;
	op_1.Len.h = 190;
	op_1.Tex = texManager.GetTexture("data/mat/img/option_01.png", op_1.Len.w, op_1.Len.h, NULL);
	D3DXMatrixTranslation(&op_1.Mat, TEXW(op_1.Len.w) / 2.0f + TEXW(op_1.Len.w) / 7.0f - offset, TEXH(op_1.Len.h) / 3.5f, 0);

	op_BGM.Len.w = 143;
	op_BGM.Len.h = 75;
	op_BGM.Tex = texManager.GetTexture("data/mat/img/option_bgm.png", op_BGM.Len.w, op_BGM.Len.h, NULL);
	D3DXMatrixTranslation(&op_BGM.Mat, TEXW(op_BGM.Len.w) / 2.84f, TEXH(op_BGM.Len.h) / 1.75f, 0);

	op_SE.Len.w = 95;
	op_SE.Len.h = 75;
	op_SE.Tex = texManager.GetTexture("data/mat/img/option_se.png", op_SE.Len.w, op_SE.Len.h, NULL);
	D3DXMatrixTranslation(&op_SE.Mat, TEXW(op_SE.Len.w) / 2.75f, TEXH(op_SE.Len.h) / 2.0f + TEXH(op_SE.Len.h) / 4.0f, 0);

	op_Vol.Tex = texManager.GetTexture("data/mat/img/number.png", 240, 42, NULL);

	op_back.Len.w = 194;
	op_back.Len.h = 84;
	op_back.Tex = texManager.GetTexture("data/mat/img/back_message.png", op_back.Len.w, op_back.Len.h, NULL);
	D3DXMatrixTranslation(&op_back.Mat, SCRW / 2.0f, SCRH - SCRH / 8.0f, 0);

	D3DXMatrixTranslation(&tmpShadowMat, 4, 4, 0);
#pragma endregion

#pragma region ポリゴン
	////　ポリゴン
	for (int i = 0; i < volNum; i++)
	{
		RectPolygonCreate(volVer[i], 24, 42, 0.028f);

		volVer[i][0].Color = D3DCOLOR_ARGB(255, 77, 77, 77);
		volVer[i][1].Color = D3DCOLOR_ARGB(255, 77, 77, 77);
		volVer[i][2].Color = D3DCOLOR_ARGB(255, 255, 255, 255);
		volVer[i][3].Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
#pragma endregion

#pragma region サウンド
	volBGM = *taskManager.BGMGetter();
	volSE = *taskManager.SEGetter();

	if (s_snd != nullptr || s_3Dsnd != nullptr)
	{
		bgmSnd = s_snd;
		bgm3DSnd = s_3Dsnd;
	}
	else
	{
		sndManager.GetSound(bgmSnd, bgm3DSnd, "data/mat/snd/bgm_2.wav");
		Vol = SoundManager::SoundVolume(volBGM, 95.0f);
		bgmSnd->SetVolume((LONG)Vol);
	}

	sndManager.GetSound(decSnd, dec3DSnd, "data/mat/snd/decision1.wav");
	Vol = SoundManager::SoundVolume(volSE, 100.0f);
	decSnd->SetVolume((LONG)Vol);

	sndManager.GetSound(chSnd, ch3DSnd, "data/mat/snd/cursor1.wav");
	Vol = SoundManager::SoundVolume(volSE, 100.0f);
	chSnd->SetVolume((LONG)Vol);
#pragma endregion

	////　その他
	if (bg == nullptr) bg = new ObjectBack();

	int bit = 0x0;
	if (*taskManager.CursorPitchGetter() != 1)	bit |= 0x1;
	if (*taskManager.CursorYawGetter() != 1)	bit |= 0x2;
	crsrNum = bit;

	while (c_cnt < 0) c_cnt = ShowCursor(TRUE);
}
SceneOption::~SceneOption()
{
	if (bg != nullptr) delete bg;
	//　テクスチャ
	SafeRelease(op_Msg.Tex);
	SafeRelease(slctTex);
	SafeRelease(op_Mouse.Tex);
	SafeRelease(op_1.Tex);
	SafeRelease(op_BGM.Tex);
	SafeRelease(op_SE.Tex);
	SafeRelease(op_Vol.Tex);

	//　サウンド
	SafeRelease(bgmSnd);
	SafeRelease(bgm3DSnd);
	SafeRelease(chSnd);
	SafeRelease(ch3DSnd);
	SafeRelease(decSnd);
	SafeRelease(dec3DSnd);

}

////　　　　　選択時　　　　　////
void SceneOption::Cursor()
{
	slctPos = D3DXVECTOR2(TEXW(s_TexLen.w) / 3.0f, TEXH(s_TexLen.h) / 3.0f);

	const float offset = -50.0f;
	POINT Pt, tmpPt;
	GetCursorPos(&Pt);
	tmpPt.x = 0;
	tmpPt.y = 0;
	ClientToScreen(hwnd, &tmpPt);

	////　　　　　キー操作　　　　　////
	if (GetAsyncKeyState('A') & 0x8000)
	{
		w_keyFlg[0] = 1;
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		w_keyFlg[0] = 2;
	}

	////　　　　　マウス操作　　　　　////
	else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (Pt.x >= tmpPt.x + symLen - slctPos.x + 50.0f + offset &&
			Pt.x <= tmpPt.x + symLen - slctPos.x + 130.0 + offset)
		{
			w_keyFlg[0] = 1;
		}
		else if (Pt.x >= tmpPt.x + symLen + slctPos.x + 50.0f + offset &&
			Pt.x <= tmpPt.x + symLen + slctPos.x + 130.0f + offset)
		{
			w_keyFlg[0] = 2;
		}
	}
	else w_keyFlg[0] = 0;

	switch (w_keyFlg[0])
	{
	case 1:
		if (w_keyFlg[1] == (int)false)
		{
			crsrNum--;
			if (crsrNum < 0) crsrNum = 0;
			else
			{
				decSnd->SetCurrentPosition(0);
				decSnd->Play(0, 0, 0);
			}
		}
		w_keyFlg[1] = (int)true;
		break;

	case 2:
		if (w_keyFlg[1] == (int)false)
		{
			crsrNum++;
			if (crsrNum >= crsrMax) crsrNum = crsrMax - 1;
			else
			{
				decSnd->SetCurrentPosition(0);
				decSnd->Play(0, 0, 0);
			}
		}
		w_keyFlg[1] = (int)true;
		break;

	default:
		w_keyFlg[1] = (int)false;
		break;
	}
}
void SceneOption::Volume(int *g_Vol)
{
	const float offset = -50.0f;
	POINT Pt, tmpPt;
	GetCursorPos(&Pt);
	tmpPt.x = 0;
	tmpPt.y = 0;
	ClientToScreen(hwnd, &tmpPt);

	////　　　　　キー操作　　　　　////
	if (GetAsyncKeyState('A') & 0x8000)			w_keyFlg[0] = 1;
	else if (GetAsyncKeyState('D') & 0x8000)	w_keyFlg[0] = 2;

	////　　　　　マウス操作　　　　　////
	else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (Pt.x >= tmpPt.x + symLen - slctPos.x + 50.0f + offset &&
			Pt.x <= tmpPt.x + symLen - slctPos.x + 130.0f + offset)
		{
			w_keyFlg[0] = 1;
		}
		else if (Pt.x >= tmpPt.x + symLen + slctPos.x + 50.0f + offset &&
			Pt.x <= tmpPt.x + symLen + slctPos.x + 130.0f + offset)
		{
			w_keyFlg[0] = 2;
		}
		else w_keyFlg[0] = 0;
	}
	else w_keyFlg[0] = 0;

	switch (w_keyFlg[0])
	{
	case 1:
		if (cnt_hold_l <= 0)
		{
			if (w_keyFlg[1] == 0)		cnt_hold_l = 15 * 60;
			else if (w_keyFlg[1] == 1)	cnt_hold_l = 3 * 60;

			*g_Vol -= 1;
			if (*g_Vol < 0) *g_Vol = 0;
			else
			{
				decSnd->SetCurrentPosition(0);
				decSnd->Play(0, 0, 0);
			}
		}
		w_keyFlg[1] = 1;
		break;

	case 2:
		if (cnt_hold_r <= 0)
		{
			if (w_keyFlg[1] == 0)		cnt_hold_r = 15 * 60;
			else if (w_keyFlg[1] == 1)	cnt_hold_r = 3 * 60;

			*g_Vol += 1;
			if (*g_Vol > volMax) *g_Vol = volMax;
			else
			{
				decSnd->SetCurrentPosition(0);
				decSnd->Play(0, 0, 0);
			}
		}
		w_keyFlg[1] = 1;
		break;

	default:
		w_keyFlg[1] = 0;
		cnt_hold_l = cnt_hold_r = 0;
		break;
	}
	cnt_hold_l--;
	if (cnt_hold_l < 0) cnt_hold_l = 0;
	cnt_hold_r--;
	if (cnt_hold_r < 0) cnt_hold_r = 0;

}
void SceneOption::BackScene()
{
	slctPos = D3DXVECTOR2(SCRW, SCRH);

	float tmpAng;
	D3DXMATRIX tmpTransMat;
	tmpAng = 1.0f + sin(D3DXToRadian(triAng / 3.5f)) / 4.0f;
	D3DXMatrixScaling(&sclMat, tmpAng, tmpAng, 0.0f);
}

bool SceneOption::Update()
{
	ShowCursor(TRUE);
	ShowCursor(FALSE);

	hwnd = GetActiveWindow();
	if (hwnd == NULL) return true;

	if (bg->Moving() == false) return false;

#pragma region 選択処理
	POINT Pt, tmpPt;
	GetCursorPos(&Pt);
	tmpPt.x = 0;
	tmpPt.y = 0;
	ClientToScreen(hwnd, &tmpPt);

	////　　　キー操作　　　////
	if (GetAsyncKeyState('W') & 0x8000)
	{
		if (h_keyFlg == false)
		{
			h_keyFlg = true;
			slctNum--;

			if (slctNum < 0) slctNum = 0;
			else
			{
				chSnd->SetCurrentPosition(0);
				chSnd->Play(0, 0, 0);
			}
		}
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		if (h_keyFlg == false)
		{
			h_keyFlg = true;
			slctNum++;

			if (slctNum >= slctMax) slctNum = slctMax - 1;
			else
			{
				chSnd->SetCurrentPosition(0);
				chSnd->Play(0, 0, 0);
			}
		}
	}
	else h_keyFlg = false;

	////　　　マウス操作　　　////
	const D3DXVECTOR2 offset(0, -75);
	if (Pt.x >= tmpPt.x + SCRW / 2.0f - TEXW(s_TexLen.w) / 4.0f + offset.x &&
		Pt.x <=tmpPt.x + SCRW / 2.0f + TEXW(s_TexLen.w) / 4.0f + offset.x)
	{
		if (Pt.y >= tmpPt.y + TEXH(op_Mouse.Len.h) / 2.8f + offset.y &&
			Pt.y <= tmpPt.y + SCRH / 1.72f + offset.y)
		{
			if (slctNum != 0)
			{
				chSnd->SetCurrentPosition(0);
				chSnd->Play(0, 0, 0);
			}
			slctNum = 0;
		}
		else if (Pt.y >= tmpPt.y + SCRH / 1.68f + offset.y &&
			Pt.y <= tmpPt.y + SCRH / 1.42f + offset.y)
		{
			if (slctNum != 1)
			{
				chSnd->SetCurrentPosition(0);
				chSnd->Play(0, 0, 0);
			}
			slctNum = 1;
		}
		else if (Pt.y >= tmpPt.y + SCRH / 1.38f + offset.y &&
			Pt.y <= tmpPt.y + SCRH / 1.12f + offset.y)
		{
			if (slctNum != 2)
			{
				chSnd->SetCurrentPosition(0);
				chSnd->Play(0, 0, 0);
			}
			slctNum = 2;
		}
		else if (Pt.y >= tmpPt.y + SCRH / 1.08f + offset.y &&
			Pt.y <= tmpPt.y + SCRH / 0.9f + offset.y)
		{
			if (slctNum != 3)
			{
				chSnd->SetCurrentPosition(0);
				chSnd->Play(0, 0, 0);

				triAng = 0;
			}
			slctNum = 3;
		}
	}
#pragma endregion

#pragma region 更新処理
	D3DXMatrixIdentity(&sclMat);
	switch (slctNum)
	{
	case 0:
		Cursor();
		break;

	case 1:
		slctPos = D3DXVECTOR2(TEXW(s_TexLen.w) / 3.0f, TEXH(s_TexLen.h) / 1.77f);

		Volume(&volBGM);
		Vol = SoundManager::SoundVolume(volBGM, 95.0f);
		bgmSnd->SetVolume((LONG)Vol);
		break;

	case 2:
		slctPos = D3DXVECTOR2(TEXW(s_TexLen.w) / 3.0f, TEXH(s_TexLen.h) / 1.34f);

		Volume(&volSE);
		Vol = SoundManager::SoundVolume(volSE, 100.0f);
		chSnd->SetVolume((LONG)Vol);
		Vol = SoundManager::SoundVolume(volSE, 100.0f);
		decSnd->SetVolume((LONG)Vol);
		break;
	case 3:
		BackScene();
		break;
	}
	triAng++;
#pragma endregion

#pragma region シーン切替
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (scnKeyFlg == false)
		{
			scnKeyFlg = true;

			scnManager.ChangeScene(new SceneSelect(bgmSnd, bgm3DSnd));
			return false;
		}
	}
	else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (scnKeyFlg == false)
		{
			if (slctNum != 3) return true;

			decSnd->SetCurrentPosition(0);
			decSnd->Play(0, 0, 0);

			int x_crsr, y_crsr;
			if (crsrNum & 0x1)	x_crsr = -1;
			else 				x_crsr = 1;
			if (crsrNum & 0x2)	y_crsr = -1;
			else 				y_crsr = 1;

			taskManager.CursorPitchSetter(x_crsr);
			taskManager.CursorYawSetter(y_crsr);

			taskManager.BGMSetter(volBGM);
			taskManager.SESetter(volSE);

			scnKeyFlg = true;

			scnManager.ChangeScene(new SceneSelect(bgmSnd, bgm3DSnd));
			return false;
		}
	}
	else scnKeyFlg = false;
#pragma endregion

	return true;
}

void SceneOption::Render3D()
{
	bg->Render3D();

	//　音量(BGM,SE含む)
	char volPer[7];
	int r_cnt = 0;
	sprintf_s(volPer, sizeof(volPer), "%03d%03d\0", volBGM, volSE);

#pragma region BGM,SEの音量表示
	for (int i = 0; i < 2; i++)
	{
		const float offset = i * 1.9f;
		for (int j = 0; j < volNum; j++)
		{
			//　音量
			int No = volPer[r_cnt] - '0';
			r_cnt++;

			if (j != 2 && No == 0)
			{
				if (j == 0) continue;
				if (volPer[r_cnt - 2] == '0') continue;
			}

			volVer[j][0].Tex = D3DXVECTOR2((24.0f * No) / 240.0f, 0 / 42.0f);
			volVer[j][1].Tex = D3DXVECTOR2((24.0f * (No + 1)) / 240.0f, 0 / 42.0f);
			volVer[j][2].Tex = D3DXVECTOR2((24.0f * (No + 1)) / 240.0f, 42 / 42.0f);
			volVer[j][3].Tex = D3DXVECTOR2((24.0f * No) / 240.0f, 42 / 42.0f);

			lpD3DDevice->SetTexture(0, op_Vol.Tex);
			D3DXMatrixTranslation(&op_Vol.Mat, (float)j, -0.7f - offset, 0);
			lpD3DDevice->SetTransform(D3DTS_WORLD, &op_Vol.Mat);

			lpD3DDevice->SetFVF(FVF_VERTEX);
			lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &volVer[j], sizeof(VERTEX));
		}
	}
#pragma endregion
}
void SceneOption::Render2D()
{
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	RECT rcOption = { 0,0,(LONG)op_Msg.Len.w,(LONG)op_Msg.Len.h };
	Draw2D(&(op_Msg.Mat * tmpShadowMat), op_Msg.Tex, &rcOption, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&op_Msg.Mat, op_Msg.Tex, &rcOption, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	int tmpInv = 1;
	for (int i = 0; i < slctImg; i++)
	{
		D3DXMATRIX tmpTransMat;
		D3DXMatrixTranslation(&tmpTransMat, symLen + slctPos.x * -tmpInv, slctPos.y, 0);
		const float tmpInt = s_TexLen.w / 2.0f;
		RECT rcSelect = { (LONG)(tmpInt * i),0,(LONG)(tmpInt * (i + 1)),(LONG)(s_TexLen.h) };

		D3DXMATRIX tmpMat = tmpTransMat;
		tmpMat._41 = tmpTransMat._41 + cos(D3DXToRadian(triAng / 3.5f)) * shake * tmpInv;

		Draw2D(&(tmpMat * tmpShadowMat), slctTex, &rcSelect, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
		Draw2D(&tmpMat, slctTex, &rcSelect, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		tmpInv *= -1;
	}


	RECT rcMouse = { 0,0,(LONG)op_Mouse.Len.w,(LONG)op_Mouse.Len.h };
	Draw2D(&(op_Mouse.Mat * tmpShadowMat), op_Mouse.Tex, &rcMouse, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&op_Mouse.Mat, op_Mouse.Tex, &rcMouse, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMATRIX tmpMat;
	const float Rect_Y = op_1.Len.h / 2.0f;

	D3DXMatrixIdentity(&tmpMat);
	tmpMat._42 = Rect_Y;

	RECT rcOption_1;
	if (crsrNum & 0x1)	rcOption_1 = { 0,(LONG)Rect_Y,(LONG)op_1.Len.w,(LONG)(Rect_Y * 2) };
	else 				rcOption_1 = { 0,0,(LONG)op_1.Len.w,(LONG)Rect_Y };
	Draw2D(&(op_1.Mat * tmpShadowMat), op_1.Tex, &rcOption_1, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&op_1.Mat, op_1.Tex, &rcOption_1, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	if (crsrNum & 0x2)	rcOption_1 = { 0,(LONG)Rect_Y,(LONG)op_1.Len.w,(LONG)(Rect_Y * 2) };
	else 				rcOption_1 = { 0,0,(LONG)op_1.Len.w,(LONG)Rect_Y };
	Draw2D(&((op_1.Mat * tmpMat) * tmpShadowMat), op_1.Tex, &rcOption_1, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&(op_1.Mat * tmpMat), op_1.Tex, &rcOption_1, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));


	RECT rcBGM = { 0,0,(LONG)op_BGM.Len.w,(LONG)op_BGM.Len.h };
	Draw2D(&(op_BGM.Mat * tmpShadowMat), op_BGM.Tex, &rcBGM, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&op_BGM.Mat, op_BGM.Tex, &rcBGM, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));


	RECT rcSE = { 0,0,(LONG)op_SE.Len.w,(LONG)op_SE.Len.h };
	Draw2D(&(op_SE.Mat * tmpShadowMat), op_SE.Tex, &rcSE, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&op_SE.Mat, op_SE.Tex, &rcSE, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT rcBack = { 0,0,(LONG)op_back.Len.w,(LONG)op_back.Len.h };
	Draw2D(&(sclMat * op_back.Mat * tmpShadowMat), op_back.Tex, &rcBack, &D3DXVECTOR3(op_back.Len.w / 2.0f, op_back.Len.h / 2.0f, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&(sclMat * op_back.Mat), op_back.Tex, &rcBack, &D3DXVECTOR3(op_back.Len.w / 2.0f, op_back.Len.h / 2.0f, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	lpSprite->End();
}
#include"../header/SceneTitle.h"

extern SceneManager	scnManager;

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr) return;
	lpAdd->Release();
	lpAdd = nullptr;
}

SceneTitle::SceneTitle()
{
	SetLight();

	tit_back.Len.w = 1280;
	tit_back.Len.h = 720;
	tit_back.Tex = texManager.GetTexture("data/mat/img/title_bg.png", 2048, 1024, NULL);
	D3DXMatrixTranslation(&tit_back.Mat, 0, 0, 0);

	tit_logo.Len.w = 1024;
	tit_logo.Len.h = 128;
	tit_logo.Tex = texManager.GetTexture("data/mat/img/title_logo.png", 1024, 128, NULL);
	D3DXMatrixTranslation(&tit_logo.Mat,
		TEXW(tit_logo.Len.w) / 2.0f,
		TEXH(tit_logo.Len.h) / 4.0f,
		0);

	st_mes.Len.w = 512;
	st_mes.Len.h = 64;
	st_mes.Tex = texManager.GetTexture("data/mat/img/start_message.png", 512, 64, NULL);
	baseY = TEXH(st_mes.Len.h) / 2.0f + TEXH(st_mes.Len.h) / 4.0f;
	D3DXMatrixTranslation(&st_mes.Mat, TEXW(st_mes.Len.w) / 2.0f, baseY, 0);

	quit_logo.Len.w = 674;
	quit_logo.Len.h = 247;
	quit_logo.Tex = texManager.GetTexture("data/mat/img/escape_message.png", 1024, 256, NULL);
	D3DXMatrixTranslation(&quit_logo.Mat,
		TEXW(quit_logo.Len.w) / 2.0f, 
		TEXH(quit_logo.Len.h / 3.0f) / 2.0f,
		0);

	fade.Len.w = fade.Len.h = 3;
	fade.Tex = texManager.GetTexture("data/mat/img/sprite.png", 4, 4, NULL);

	D3DXMatrixTranslation(&tmpShadowMat, 4, 4, 0);

	q_keyFlg = false;
	stFlg = false;
	quitFlg = 0;
	q_fade = 0;

	sndManager.GetSound(bgmSnd, bgm3DSnd, "data/mat/snd/bgm_1.wav");

	Vol = SoundManager::SoundVolume(*taskManager.BGMGetter(), 95.0f);
	bgmSnd->SetVolume((LONG)Vol);
	bgmSnd->Play(0, 0, DSBPLAY_LOOPING);

	sndManager.GetSound(stSnd, st3DSnd, "data/mat/snd/se_start.wav");

	Vol = SoundManager::SoundVolume(*taskManager.SEGetter(), 100.0f);
	stSnd->SetVolume((LONG)Vol);

	while (c_cnt < 0) c_cnt = ShowCursor(TRUE);
}
SceneTitle::~SceneTitle()
{
	//　テクスチャ
	SafeRelease(tit_back.Tex);
	SafeRelease(tit_logo.Tex);
	SafeRelease(st_mes.Tex);
	SafeRelease(quit_logo.Tex);

	//　サウンド
	SafeRelease(bgmSnd);
	SafeRelease(bgm3DSnd);
	SafeRelease(stSnd);
	SafeRelease(st3DSnd);
}


bool SceneTitle::Update()
{
	hwnd = GetActiveWindow();

	if (hwnd == NULL) return true;

	//　操作
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (scnKeyFlg == false && q_keyFlg == false)
		{
			stSnd->SetCurrentPosition(0);
			scnKeyFlg = true;
			stFlg = true;
		}
	}
	else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (scnKeyFlg == false && stFlg == false)
		{
			scnKeyFlg = true;
			q_keyFlg = true;
		}
	}
	else scnKeyFlg = false;

#pragma region シーン切替
	if (stFlg && q_keyFlg == false)
	{
		stSnd->Play(0, 0, 0);
		if (f_a >= 255)
		{
			bgmSnd->Stop();
			bgmSnd->SetCurrentPosition(0);
			scnManager.ChangeScene(new SceneLoad(2));
			return false;
		}
		f_a++;
	}
#pragma endregion

#pragma region 終了処理
	if (q_keyFlg)
	{
		const D3DXVECTOR2 offset(-50, -75);
		bool tmpFlg = false;
		POINT Pt, tmpPt;
		GetCursorPos(&Pt);
		tmpPt.x = 0;
		tmpPt.y = 0;
		ClientToScreen(hwnd, &tmpPt);

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			tmpFlg = true;
			quitFlg = 1;
		}
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			tmpFlg = true;
		}
		////　　　　　カーソル選択　　　　　////
		if (Pt.y >= tmpPt.y + quit_logo.Mat._42 + quit_logo.Len.h / 2.0f &&
			Pt.y <= tmpPt.y + quit_logo.Mat._42 + quit_logo.Len.h / 1.3)
		{
			if (Pt.x >= tmpPt.x + quit_logo.Mat._41 + 100 &&
				Pt.x <= tmpPt.x + quit_logo.Mat._41 + quit_logo.Len.w / 2.0f - 100)
			{
				if (quitFlg != 1) q_triAng = 0;
				quitFlg = 1;
				q_triAng++;
			}
			else if (Pt.x >= tmpPt.x + quit_logo.Mat._41 + quit_logo.Len.w / 2.0f + 60 &&
				Pt.x <= tmpPt.x + quit_logo.Mat._41 + quit_logo.Len.w - 60)
			{
				if (quitFlg != 2) q_triAng = 0;
				quitFlg = 2;
				q_triAng++;
			}
			else quitFlg = 0;
		}
		else quitFlg = 0;

		if (tmpFlg)
		{
			switch (quitFlg)
			{
			case 1:
				PostQuitMessage(WM_QUIT);
				break;
			case 2:
				scnKeyFlg = true;
				q_keyFlg = false;
				quitFlg = 0;
				break;
			default:
				break;
			}
		}

		if (qf_cnt >= 60 / 4)
		{
			q_fade++;
			if (q_fade > (int)(255 / 2.5f)) q_fade = (int)(255 / 2.5f);
			qf_cnt = 0;
		}
		qf_cnt++;
	}
	else
	{
		q_fade = 0;
		qf_cnt = 0;
	}
#pragma endregion

	st_mes.Mat._42 = baseY + sin(D3DXToRadian(triAng / 2.5f))*shake;
	triAng++;
	return true;
}

void SceneTitle::Render2D()
{
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	RECT rcBack = { 0,0,(LONG)tit_back.Len.w,(LONG)tit_back.Len.h };

	const int t_tmpRGB = 255 / 2 + 255 / 4;
	Draw2D(&tit_back.Mat, tit_back.Tex, &rcBack, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, t_tmpRGB - q_fade, t_tmpRGB - q_fade, t_tmpRGB - q_fade));

	D3DXCOLOR tmpARGB_1(D3DCOLOR_ARGB(255, 255 - q_fade, 255 - q_fade, 255 - q_fade));
	RECT rcLogo = { 0,0,(LONG)tit_logo.Len.w,(LONG)tit_logo.Len.w };
	Draw2D(&tit_logo.Mat, tit_logo.Tex, &rcLogo, &D3DXVECTOR3(0, 0, 0), tmpARGB_1);

	RECT rcStart = { 0,0,(LONG)st_mes.Len.w,(LONG)st_mes.Len.h };
	Draw2D(&st_mes.Mat, st_mes.Tex, &rcStart, &D3DXVECTOR3(0, 0, 0), tmpARGB_1);
	
	if (q_keyFlg)
	{
		RECT rcEscape = { 0,0,(LONG)quit_logo.Len.w,(LONG)(quit_logo.Len.h / 3.0f) };
		D3DXMATRIX tmpMat;

		tmpMat = quit_logo.Mat;
		tmpMat._42 -= quit_logo.Len.h / 2.0f;
		Draw2D(&(tmpMat * tmpShadowMat), quit_logo.Tex, &rcEscape, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
		Draw2D(&tmpMat, quit_logo.Tex, &rcEscape, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

		for (int i = 0; i < 2; i++)
		{
			RECT rcYesNo = {
				(LONG)(quit_logo.Len.w / 2.0f*i),
				(LONG)(quit_logo.Len.h / 3.0f * 2),
				(LONG)(quit_logo.Len.w / 2.0f*(i + 1)),
				(LONG)(quit_logo.Len.h) };

			tmpMat = quit_logo.Mat;
			tmpMat._41 += quit_logo.Len.w / 2.0f * i + 160;
			tmpMat._42 += quit_logo.Len.h - 75;

			float t_triAng = 0;
			t_triAng += 1.0f + sin(D3DXToRadian(q_triAng / 2.5f)) * 0.25f;
			D3DXMatrixScaling(&q_sclMat, t_triAng, t_triAng, 0);

			D3DXMATRIX t_TransMat;
			D3DXMatrixTranslation(&t_TransMat, 0, -50, 0);
			q_sclMat = t_TransMat * q_sclMat;
			D3DXMatrixInverse(&t_TransMat, NULL, &t_TransMat);
			q_sclMat *= t_TransMat;

			if (quitFlg == i + 1)
			{
				D3DXVECTOR3 tmpCenter(quit_logo.Len.w / 4.0f, quit_logo.Len.h / 6.0f, 0);
				Draw2D(&(q_sclMat * tmpMat * tmpShadowMat), quit_logo.Tex, &rcYesNo, &tmpCenter, D3DCOLOR_ARGB(50, 0, 0, 0));
				Draw2D(&(q_sclMat * tmpMat), quit_logo.Tex, &rcYesNo, &tmpCenter, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				D3DXVECTOR3 tmpCenter(quit_logo.Len.w / 4.0f, quit_logo.Len.h / 6.0f, 0);
				Draw2D(&(tmpMat * tmpShadowMat), quit_logo.Tex, &rcYesNo, &tmpCenter, D3DCOLOR_ARGB(50, 0, 0, 0));
				Draw2D(&tmpMat, quit_logo.Tex, &rcYesNo, &tmpCenter, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}

	RECT rcFade = { 0,0,(LONG)fade.Len.w,(LONG)fade.Len.h };
	D3DXMATRIX  scalMat;
	D3DXMatrixScaling(&scalMat, SCRW, SCRH, 0);
	Draw2D(&scalMat, fade.Tex, &rcFade, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(f_a, 255, 255, 255));

	lpSprite->End();
}
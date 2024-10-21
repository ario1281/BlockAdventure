#include "../header/SceneSelect.h"

extern SceneManager		scnManager;

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr)	return;

	lpAdd->Release();
	lpAdd = nullptr;
};

SceneSelect::SceneSelect(const LPDIRECTSOUNDBUFFER8 s_snd, const LPDIRECTSOUND3DBUFFER8 s_3Dsnd)
{
	SetLight();

	////　テクスチャ
	stgState = new StageState[stgNo];
	s_logoPos = new D3DXVECTOR3[stgNo];
	s_logoMat = new D3DXMATRIX[stgNo];

	stgState[0].Tex = texManager.GetTexture("data/mat/img/stage_01.png", 200, 235, NULL);

	s_logoTexLen.w = 225;
	s_logoTexLen.h = 75;
	s_logoTex = texManager.GetTexture("data/mat/img/stage_message.png", 256, 128, NULL);
	
	s_NumTexLen.w = 400;
	s_NumTexLen.h = 60;
	s_NumTex = texManager.GetTexture("data/mat/img/stage_number.png", 1024, 64, NULL);
	for (int i = 0; i < stgNo; i++)
	{
		D3DXMatrixIdentity(&sumRotMat);

		D3DXMatrixTranslation(&stgState[i].TransMat, (i - 1) * 5.8f, 0, 0);

		//　ステージ 文字画像
		s_logoPos[i].x = TEXW(225) / 2.0f + (i - 1) * SCRW / 3.5f - 49;
		s_logoPos[i].y = STAGE_LOGO;
		s_logoPos[i].z = 0;
	}

	op_logo.Len.w = 335;
	op_logo.Len.h = 105;
	op_logo.Tex = texManager.GetTexture("data/mat/img/option_message.png", 512, 128, NULL);
	D3DXMatrixTranslation(&op_logo.Mat, TEXW(op_logo.Len.w) / 2.0f, TEXH(op_logo.Len.h) / 2.0f + TEXH(op_logo.Len.h) / 2.5f, 0);

	stg_slct.Len.w = 640;
	stg_slct.Len.h = 161;
	stg_slct.Tex = texManager.GetTexture("data/mat/img/stage_select.png", 1024, 256, NULL);
	D3DXMatrixTranslation(&stg_slct.Mat, TEXW(stg_slct.Len.w) / 2.0f, 0, 0);

	fade.Len.w = fade.Len.h = 3;
	fade.Tex = texManager.GetTexture("data/mat/img/sprite.png", 4, 4, NULL);

	D3DXMatrixTranslation(&tmpShadowMat, 4, 4, 0);
	
	////　ポリゴン
	for (int i = 0; i < stgNo; i++) RectPolygonCreate(stgState[i].Ver, 200, 235, 0.02f);

	////　サウンド
	bgmSnd = s_snd;
	bgm3DSnd = s_3Dsnd;
	if (bgmSnd == nullptr || bgm3DSnd == nullptr)
		if (rand() % 100 < 50) sndManager.GetSound(bgmSnd, bgm3DSnd, "data/mat/snd/bgm_2.1.wav");
		else sndManager.GetSound(bgmSnd, bgm3DSnd, "data/mat/snd/bgm_2.2.wav");

	Vol = SoundManager::SoundVolume(*taskManager.BGMGetter(), 95.0f);
	bgmSnd->SetVolume((LONG)Vol);
	bgmSnd->Play(0, 0, DSBPLAY_LOOPING);

	sndManager.GetSound(chSnd, ch3DSnd, "data/mat/snd/cursor1.wav");
	Vol = SoundManager::SoundVolume(*taskManager.SEGetter(), 100.0f);
	chSnd->SetVolume((LONG)Vol);

	sndManager.GetSound(decSnd, dec3DSnd, "data/mat/snd/decision1.wav");
	Vol = SoundManager::SoundVolume(*taskManager.SEGetter(), 100.0f);
	decSnd->SetVolume((LONG)Vol);

	////　その他
	if (bg == nullptr) bg = new ObjectBack();

	stgPos = D3DXVECTOR3(-5, 1, 0);
	stgNum = 1;

	det_1 = 1;

	while (c_cnt < 0) c_cnt = ShowCursor(TRUE);
}
SceneSelect::~SceneSelect()
{
	if (bg != nullptr) delete bg;
	delete s_logoPos;

	SafeRelease(bgmSnd);
	SafeRelease(bgm3DSnd);
	SafeRelease(bgmSnd);
	SafeRelease(bgm3DSnd);
	SafeRelease(chSnd);
	SafeRelease(ch3DSnd);
	SafeRelease(decSnd);
	SafeRelease(dec3DSnd);
}

bool SceneSelect::Update()
{
	hwnd = GetActiveWindow();
	if (hwnd == NULL) return true;

	if (bg->Moving() == false) return false;

	if (f_a <= 0)
	{

#pragma region 選択処理
		////　　　キー操作　　　////
		//　ステージ選択中
		if (stgNum != 0)
		{
			if (GetAsyncKeyState('A') & 0x8000)
			{
				if (keyFlg == false)
				{
					keyFlg = true;
					stgNum--;

					if (stgNum < minNum) stgNum = minNum;
					else
					{
						chSnd->SetCurrentPosition(0);
						chSnd->Play(0, 0, 0);

						D3DXMatrixIdentity(&sumRotMat);
					}
				}
				det_1 = op_Key;
			}
			else if (GetAsyncKeyState('S') & 0x8000)
			{
				if (keyFlg == false)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);

					keyFlg = true;
					stgNum = 0;
					D3DXMatrixIdentity(&sumRotMat);
				}
				det_1 = op_Key;
			}
			else if (GetAsyncKeyState('D') & 0x8000)
			{
				if (keyFlg == false)
				{
					keyFlg = true;
					stgNum++;

					if (stgNum > maxNum) stgNum = maxNum;
					else
					{
						chSnd->SetCurrentPosition(0);
						chSnd->Play(0, 0, 0);

						D3DXMatrixIdentity(&sumRotMat);
					}
				}
				det_1 = op_Key;
			}
			else if (keyFlg) keyFlg = false;
		}
		//　オプション選択中
		else
		{
			if (GetAsyncKeyState('W') & 0x8000)
			{
				if (keyFlg == false)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);

					keyFlg = true;
					stgNum = 2;
					D3DXMatrixIdentity(&sumRotMat);
				}
				det_1 = op_Key;
			}
			else if (GetAsyncKeyState('A') & 0x8000)
			{
				if (keyFlg == false)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);

					keyFlg = true;
					stgNum = 1;

					D3DXMatrixIdentity(&sumRotMat);
				}
				det_1 = op_Key;
			}
			else if (GetAsyncKeyState('D') & 0x8000)
			{
				if (keyFlg == false)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);

					keyFlg = true;
					stgNum = 3;

					D3DXMatrixIdentity(&sumRotMat);
				}
				det_1 = op_Key;
			}
			else if (keyFlg) keyFlg = false;
		}

		////　　　マウス操作　　　////
		const D3DXVECTOR2 offset(-50, -75);
		POINT Pt, tmpPt;
		GetCursorPos(&Pt);
		tmpPt.x = 0;
		tmpPt.y = 0;
		ClientToScreen(hwnd, &tmpPt);

		//　ステージ選択中
		if (Pt.y >= tmpPt.y + (SCRH - 150) / 2.0f + offset.y && Pt.y <= tmpPt.y + (SCRH + 450) / 2.0f + offset.y)
		{
			const float stg_offset = SCRW / 4.0f + 50;
			if (Pt.x >= tmpPt.x + ((SCRW - 200) / 2.0f) - stg_offset + offset.x &&
				Pt.x <= tmpPt.x + ((SCRW + 400) / 2.0f) - stg_offset + offset.x)
			{
				if (stgNum != 1)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);

					D3DXMatrixIdentity(&sumRotMat);
				}
				stgNum = 1;

				renFlg = true;
				det_1 = op_Crsr;
			}
			else if (Pt.x >= tmpPt.x + (SCRW - 200) / 2.0f + offset.x &&
				Pt.x <= tmpPt.x + (SCRW + 400) / 2.0f + offset.x)
			{
				if (stgNum != 2)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);

					D3DXMatrixIdentity(&sumRotMat);
				}
				stgNum = 2;

				renFlg = true;
				det_1 = op_Crsr;
			}
			else if (Pt.x >= tmpPt.x + ((SCRW - 200) / 2.0f) + stg_offset + offset.x &&
				Pt.x <= tmpPt.x + ((SCRW + 400) / 2.0f) + stg_offset + offset.x)
			{
				if (stgNum != 3)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);

					D3DXMatrixIdentity(&sumRotMat);
				}
				stgNum = 3;

				renFlg = true;
				det_1 = op_Crsr;
			}
			else renFlg = false;
		}
		//　オプション選択中
		else if (Pt.y >= tmpPt.y + (SCRH + 600) / 2.0f + offset.y && Pt.y <= tmpPt.y + (SCRH + 700) / 2.0f + offset.y)
		{
			if (Pt.x >= tmpPt.x + (SCRW - 200) / 2.0f + offset.x && Pt.x <= tmpPt.x + (SCRW + 425) / 2.0f + offset.x)
			{
				if (stgNum != 0)
				{
					chSnd->SetCurrentPosition(0);
					chSnd->Play(0, 0, 0);
				}
				stgNum = 0;

				renFlg = true;
				det_1 = op_Crsr;
			}
			else renFlg = false;
		}
		else renFlg = false;
#pragma endregion

#pragma region シーン切替
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (det_1 == op_Key || det_1 == op_Crsr && renFlg)
			{
				if (scnKeyFlg == false)
				{
					decSnd->SetCurrentPosition(0);
					decSnd->Play(0, 0, 0);

					switch (stgNum)
					{
					case 0:
						taskManager.StageNunberSetter(&stgNum);

						scnKeyFlg = true;

						SceneOption *p;
						p = new SceneOption(bgmSnd, bgm3DSnd);
						scnManager.ChangeScene(p);
						return false;

					case 1:
					case 2:
					case 3:
						scnKeyFlg = true;

						char FileName[32];
						sprintf_s(FileName, sizeof(FileName), "data/txt/Stage/%02d.txt", stgNum);
						FILE *fp;
						fopen_s(&fp, FileName, "r");

						if (fp != NULL)
						{
							fclose(fp);
							bgmSnd->Stop();
							bgmSnd->SetCurrentPosition(0);

							taskManager.StageNunberSetter(&stgNum);

							scnManager.ChangeScene(new SceneLoad(3));
							return false;
						}
						MessageBox(NULL, TEXT("txtファイルが存在しません"), TEXT("txtチェック　エラー：ファイル"), MB_OK);
					}
				}
			}
		}
		//else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		//{
		//	if (scnKeyFlg == false)
		//	{
		//		bgmSnd->Stop();
		//		bgmSnd->SetCurrentPosition(0);
		//		scnManager.ChangeScene(new SceneLoad(1));
		//		return false;
		//	}
		//}
		else
		{
			scnKeyFlg = false;
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			scnKeyFlg = true;

			bgmSnd->Stop();
			bgmSnd->SetCurrentPosition(0);
			scnManager.ChangeScene(new SceneLoad(1));
			return false;
		}
#pragma endregion
	
	}
	else
	{
		f_a--;
		if (f_a < 0) f_a = 0;
	}
#pragma region 更新処理
	D3DXMATRIX tmpRotMat, idenMat;
	D3DXMatrixIdentity(&idenMat);
	for (int i = 0; i < stgNo; i++) {
		s_logoPos[i].y = STAGE_LOGO + 10.0f;
		stgState[i].RotMat = idenMat;
	}
	D3DXMatrixTranslation(&op_logo.Mat, (SCRW - 335) / 2.0f, (SCRH - 105) / 2.0f + (SCRH - 105) / 2.5f, 0);

	D3DXMatrixRotationY(&tmpRotMat, D3DXToRadian(RotAng));
	sumRotMat *= tmpRotMat;

	if (stgNum == 0)
	{
		op_logo.Mat._42 += sin(D3DXToRadian(sinAng / 2.5f))*shake;
		sinAng += 0.75f;
	}
	else
	{
		sinAng = 0;
		switch (stgNum)
		{
		case 1:
			s_logoPos[stgNum - 1].y = STAGE_LOGO;
			stgState[stgNum - 1].RotMat = sumRotMat;
			break;

		case 2:
			s_logoPos[stgNum - 1].y = STAGE_LOGO;
			stgState[stgNum - 1].RotMat = sumRotMat;
			break;

		case 3:
			s_logoPos[stgNum - 1].y = STAGE_LOGO;
			stgState[stgNum - 1].RotMat = sumRotMat;
			break;
		}
	}
	for (int i = 0; i < stgNo; i++)
	{
		D3DXMatrixTranslation(&s_logoMat[i], s_logoPos[i].x, s_logoPos[i].y, s_logoPos[i].z);
		stgState[i].Mat = stgState[i].RotMat * stgState[i].TransMat;
	}
#pragma endregion
	return true;
}

void SceneSelect::Render3D()
{
	bg->Render3D();

	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < stgNo; i++)
	{
		lpD3DDevice->SetTexture(0, stgState[0].Tex);

		lpD3DDevice->SetTransform(D3DTS_WORLD, &stgState[i].Mat);
		lpD3DDevice->SetFVF(FVF_VERTEX);
		lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, stgState[i].Ver, sizeof(VERTEX));
	}
}
void SceneSelect::Render2D()
{
	D3DXMATRIX tmpTransMat;
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	RECT rcSelect = { 0,0,640,161 };
	Draw2D(&(stg_slct.Mat * tmpShadowMat), stg_slct.Tex, &rcSelect, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&stg_slct.Mat, stg_slct.Tex, &rcSelect, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT rcStgLogo = { 0,0,(LONG)s_logoTexLen.w,(LONG)s_logoTexLen.h };
	for (int i = 0; i < stgNo; i++)
	{
		Draw2D(&(s_logoMat[i] * tmpShadowMat), s_logoTex, &rcStgLogo, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
		Draw2D(&s_logoMat[i], s_logoTex, &rcStgLogo, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMATRIX ofsMat;
		D3DXMatrixTranslation(&ofsMat, 230, 0, 0);
		RECT rcStgNum = { (LONG)(s_NumTexLen.w / 10 * (i + 1)),0,(LONG)(s_NumTexLen.w / 10 * (i + 2)),(LONG)s_NumTexLen.h };
		Draw2D(&(s_logoMat[i] * ofsMat * tmpShadowMat), s_NumTex, &rcStgNum, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
		Draw2D(&(s_logoMat[i] * ofsMat), s_NumTex, &rcStgNum, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	RECT rcOption = { 0,0,(LONG)op_logo.Len.w,(LONG)op_logo.Len.h };
	Draw2D(&(op_logo.Mat * tmpShadowMat), op_logo.Tex, &rcOption, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(50, 0, 0, 0));
	Draw2D(&op_logo.Mat, op_logo.Tex, &rcOption, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT rcFade = { 0,0,(LONG)fade.Len.w,(LONG)fade.Len.h };
	D3DXMATRIX  scalMat;
	D3DXMatrixScaling(&scalMat, SCRW, SCRH, 0.0f);
	Draw2D(&scalMat, fade.Tex, &rcFade, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(f_a, 255, 255, 255));

	lpSprite->End();
}

void SceneSelect::SetCamera()
{
	D3DXMATRIX mView, mProj;

	// 視点行列の設定
	D3DXMatrixLookAtLH(&mView,
		&D3DXVECTOR3(0.0f, 0.0f, -10.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)
	);

	// 投影行列の設定
	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(60), (float)SCRW / (float)SCRH, 1.0f, 1000.0f);

	//行列設定
	lpD3DDevice->SetTransform(D3DTS_VIEW, &mView);
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);

}
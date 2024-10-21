#include<vector>
#include"../header/SceneGame.h"

extern SceneManager		scnManager;

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color);

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr) return;
	lpAdd->Release();
	lpAdd = nullptr;
}


SceneGame::SceneGame()
{

#pragma region Polygons

#pragma endregion

#pragma region Sounds

	sndManager.GetSound(bgmSnd, bgm3DSnd, "data/mat/snd/bgm_3.wav");
	Vol = sndManager.SoundVolume(*taskManager.BGMGetter(), 90.0f);

	bgmSnd->SetVolume((LONG)Vol);
	bgmSnd->Play(0, 0, DSBPLAY_LOOPING);

#pragma endregion

#pragma region Stage_Create

	int stgNo = *taskManager.StageNunberGetter();

	////　　　ステージ読取　　　////
	char FileName[64];
	sprintf_s(FileName, sizeof(FileName), "data/txt/Stage/%02d.txt", stgNo);

	FILE* p_Data;
	fopen_s(&p_Data, FileName, "r");
	if (stgBase == nullptr) stgBase = new ObjectStage();

	int numID = 0;

	fseek(p_Data, 0, SEEK_SET);
	while (1)
	{
		bool cre_Flg = false;
		char objID;
		float x, y, z;
		int d1, d2, r_Flg;

		r_Flg = fscanf_s(p_Data, "%c:", &objID);

		if (r_Flg == EOF) break;

		switch (objID)
		{
		case Wood:
			fscanf_s(p_Data, "(%1f,%1f,%1f);", &x, &y, &z);
			x = ((x + (0.5f + 200 * 0.5f)) - 200);
			y = ((y + (0.5f + 200 * 0.5f)) - 200);
			z = ((z + (0.5f + 200 * 0.5f)) - 200);
			break;

		case startPos:
			if (ply_Flg == false)
			{
				fscanf_s(p_Data, "(%1f,%1f,%1f);", &x, &y, &z);
				x = ((x + (0.5f + 200 * 0.5f)) - 200);
				y = ((y + (0.5f + 200 * 0.5f)) - 200);
				z = ((z + (0.5f + 200 * 0.5f)) - 200);
			}
			else MessageBox(NULL, "すでに処理を行っています。", "WORNING", MB_OK);

			break;

		case Stone:
		case Glass:
			fscanf_s(p_Data, "(%1f,%1f,%1f);", &x, &y, &z);
			x = ((x + (0.5f + 200 * 0.5f)) - 200);
			y = ((y + (0.5f + 200 * 0.5f)) - 200);
			z = ((z + (0.5f + 200 * 0.5f)) - 200);
			break;

		case Goal:
			if (fin_Flg == false)
			{
				fscanf_s(p_Data, "(%1f,%1f,%1f,%d);", &x, &y, &z, &d1);
				x = ((x + (0.5f + 200 * 0.5f)) - 200);
				y = ((y + (0.5f + 200 * 0.5f)) - 200);
				z = ((z + (0.5f + 200 * 0.5f)) - 200);
			}
			else MessageBox(NULL, "すでに処理を行っています。", "WORNING", MB_OK);
			break;

		case Switch:
			fscanf_s(p_Data, "(%1f,%1f,%1f,%d,%d);", &x, &y, &z, &d1, &d2);
			x = ((x + (0.5f + 200 * 0.5f)) - 200);
			y = ((y + (0.5f + 200 * 0.5f)) - 200);
			z = ((z + (0.5f + 200 * 0.5f)) - 200);
			break;

		case '\n':
			break;

		default:
			MessageBox(NULL, "指定された文字が見つかりません。", "ERROR", MB_OK | MB_ICONERROR);
			PostQuitMessage(WM_QUIT);
			return;
		};

		switch (objID)
		{
		case startPos:
			if (ply_Flg == false)
			{
				player = new Human(&D3DXVECTOR3((float)x, (float)y, (float)z));
				ply_Flg = true;
			}
			fputc('\n', p_Data);
			break;

		case Wood:
			arr_wd.push_back(new ObjectWood(&D3DXVECTOR3(x, y, z), objID, numID));
			fputc('\n', p_Data);
			break;

		case Stone:
			arr_st.push_back(new ObjectStone(&D3DXVECTOR3(x, y, z), objID, numID));
			fputc('\n', p_Data);
			break;

		case Glass:
			arr_gl.push_back(new ObjectGlass(&D3DXVECTOR3(x, y, z), objID, numID));
			fputc('\n', p_Data);
			break;

		case Goal:
			if (fin_Flg == false)
			{
				fin_Flg = true;
				goal = new ObjectGoal(&D3DXVECTOR3(x, y, z), objID, numID, d1);
			}
			fputc('\n', p_Data);
			break;

		case Switch:
			arr_sw.push_back(new ObjectSwitch(&D3DXVECTOR3(x, y, z), objID, numID, d1, d2));
			fputc('\n', p_Data);
			break;
		}
		numID++;
	}
	fclose(p_Data);

	if (ply_Flg == false || fin_Flg == false)
	{
		MessageBox(NULL, "オブジェクトが足りません。", "ERROR", MB_OK | MB_ICONERROR);
		PostQuitMessage(WM_QUIT);
	}
#pragma endregion

#pragma region Other

	if (crsr == nullptr) crsr = new Cursor();
	if (bg == nullptr) bg = new ObjectBack();
	camHead = D3DXVECTOR3(0, 1, 0);
	while (c_cnt >= 0) c_cnt = ShowCursor(FALSE);

#pragma endregion

}
SceneGame::~SceneGame()
{

#pragma region Release
	if (player != nullptr) delete player;
	if (bg != nullptr) delete bg;
	if (stgBase != nullptr) delete stgBase;

	for (auto &&p : arr_wd) delete p;
	for (auto &&p : arr_st) delete p;
	for (auto &&p : arr_gl) delete p;
	for (auto &&p : arr_sw) delete p;

	arr_wd.clear();
	arr_st.clear();
	arr_gl.clear();
	arr_sw.clear();

	if (goal != nullptr) delete goal;

	//　サウンド
	SafeRelease(bgmSnd);
	SafeRelease(bgm3DSnd);
#pragma endregion

}

bool SceneGame::Update()
{
	hwnd = GetActiveWindow();
	if (hwnd == NULL)
	{
		while (c_cnt < 0) c_cnt = ShowCursor(TRUE);
		return true;
	}
	else while (c_cnt >= 0) c_cnt = ShowCursor(FALSE);
#pragma region 確認処理
	if (crsr == nullptr)	return false;
	if (bg == nullptr)	return false;
	if (stgBase == nullptr)	return false;
	if (player == nullptr)	return false;
	if (player == nullptr)	return false;
	if (goal == nullptr)	return false;
	for (auto &&p : arr_sw) if (p == nullptr)	return false;
	for (auto &&p : arr_st) if (p == nullptr)	return false;
	for (auto &&p : arr_gl) if (p == nullptr)	return false;
	for (auto &&p : arr_wd) if (p == nullptr)	return false;
#pragma endregion

	goal->Update();

#pragma region シーン切替
	if (*goal->FlagGetter())
	{
		bgmSnd->Stop();
		bgmSnd->SetCurrentPosition(0);
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (scnKeyFlg == false)
			{
				scnKeyFlg = true;
				scnManager.ChangeScene(new SceneLoad(2));
				return false;
			}
			scnKeyFlg = false;
		}
		return true;
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		scnKeyFlg = true;

		bgmSnd->Stop();
		bgmSnd->SetCurrentPosition(0);
		scnManager.ChangeScene(new SceneLoad(1));
		return false;
	}
	else scnKeyFlg = false;
#pragma endregion

#pragma region 動作処理
	if (crsr->Rotation() == false)	return false;
	if (bg->Moving() == false)	return false;
	if (player->Moving(crsr->RotYMatSetter()) == false)	return false;
	for (auto &&p : arr_gl) if (p->Breaking() == false) return false;

	bool s_Flg = false;
	for (auto &&p : arr_sw)
	{
		if (*p->FlagGetter() == true)
		{
			s_Flg = true;
			if (p->StageRotation() == false)	return false;

			D3DXMATRIX tmpMat;
			if (*player->FlagGetter() == true) player->FlagSetter(false);
			//　プレイヤー
			tmpMat = *player->MatGetter();
			if (p->AddToRotation(nullptr, &tmpMat) == false) return false;

			D3DXMatrixTranslation(&tmpMat, tmpMat._41, tmpMat._42, tmpMat._43);
			player->MatSetter(nullptr, nullptr, &tmpMat);
			//　ゴール
			if (1)
			{
				tmpMat = *goal->MatGetter();
				if (p->AddToRotation(nullptr, &tmpMat) == false) return false;
				goal->MatSetter(&tmpMat, nullptr, nullptr);
			}
			//　スイッチ
			for (auto &&q : arr_sw)
			{
				D3DXVECTOR3 tmpVec = *q->axisVecGetter();
				tmpMat = *q->MatGetter();
				if (p->AddToRotation(&tmpVec, &tmpMat) == false) return false;
				q->axisVecSetter(&tmpVec);
				q->MatSetter(&tmpMat, nullptr, nullptr);
			}
			//　ストーン
			for (auto &&q : arr_st)
			{
				tmpMat = *q->TransMatGetter();
				if (p->AddToRotation(nullptr, &tmpMat) == false) return false;
				q->MatSetter(nullptr, nullptr, &tmpMat);
			}
			//　ガラス
			for (auto &&q : arr_gl)
			{
				tmpMat = *q->MatGetter();
				if (p->AddToRotation(nullptr, &tmpMat) == false) return false;
				q->MatSetter(&tmpMat, nullptr, nullptr);
			}
			//　ウッド
			for (auto &&q : arr_wd)
			{
				tmpMat = *q->MatGetter();
				if (p->AddToRotation(nullptr, &tmpMat) == false) return false;
				q->MatSetter(&tmpMat, nullptr, nullptr);
			}
		}
	}

	if (s_Flg == false)
	{
		if (player->Falling() == false) return false;
		for (auto &&p : arr_st) if (p->Falling() == false) return false;
	}
#pragma endregion

#pragma region 判定処理
	if (s_Flg == false)
	{
		////　プレイヤー
		float offset;
		bool tmpFlg;
		const float tmpTest = 1.0f;
		D3DXMATRIX tmpTransMat = *player->TransMatGetter();
		offset = 0.3f;

		//　ゴール
		if (1)
		{
			BOOL Hit = FALSE;
			float mDis = 0;
			JudgeManager::JudgeRayYaw(&tmpTransMat, goal->MeshGetter(), goal->MatGetter(), &offset, &Hit, &mDis, 2.5f, "my");
			if (Hit == TRUE)
			{
				if (mDis < (JDG_LEN + offset))
				{
					player->FallSetter(0.0f);
					tmpTransMat._42 += (JDG_LEN + offset) - mDis;
				}
				goal->FlagSetter(true);
			}
			tmpFlg = false;
			if (JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), goal->MeshGetter(), goal->MatGetter(), 2.0f) == false) tmpFlg = true;
			if (tmpFlg)
			{
				tmpTransMat._42 += 1.0f;
				JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), goal->MeshGetter(), goal->MatGetter(), 2.0f);
				tmpTransMat._42 -= 1.0f;
			}
		}

		//　ステージ
		if (1)
		{
			BOOL Hit = FALSE;
			float mDis = 0;
			JudgeManager::JudgeMesh(&tmpTransMat, stgBase->MeshGetter(), stgBase->MatGetter(), &D3DXVECTOR3(0, -1, 0), 1.0f);
			JudgeManager::JudgeMesh(&tmpTransMat, stgBase->MeshGetter(), stgBase->MatGetter(), player->DirVecGetter(), 0.5f);
		}

		//　スイッチ
		for (auto &&p : arr_sw)
		{
			BOOL Hit = FALSE;
			float mDis = 0;
			JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), &offset, &Hit, &mDis, 2.5f, "my");
			if (Hit == TRUE)
				if (mDis < (JDG_LEN + offset))
				{
					if (*player->FlagGetter() == true)
					{
						player->FlagSetter(false);
						if (p->PreparateRotation() == false) return false;
					}
					player->FallSetter(0.0f);
					tmpTransMat._42 += (JDG_LEN + offset) - mDis;
				}
			tmpFlg = false;
			if (JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f) == false) tmpFlg = true;
			if (tmpFlg)
			{
				tmpTransMat._42 += 1.0f;
				JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f);
				tmpTransMat._42 -= 1.0f;
			}
		}
		//　ストーン
		for (auto &&p : arr_st)
		{
			BOOL Hit = FALSE;
			float mDis = 0;
			JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), &offset, &Hit, &mDis, 2.5f, "my");
			if (Hit == TRUE)
				if (mDis < (JDG_LEN + offset))
				{
					if (*player->FlagGetter() == false) player->FlagSetter(true);
					player->FallSetter(0.0f);
					tmpTransMat._42 += (JDG_LEN + offset) - mDis;
				}
			tmpFlg = false;
			if (JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f) == false) tmpFlg = true;
			if (tmpFlg)
			{
				tmpTransMat._42 += 1.0f;
				JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f);
				tmpTransMat._42 -= 1.0f;
			}
		}
		//　ガラス
		for (auto &&p : arr_gl)
		{
			BOOL Hit = FALSE;
			float mDis = 0;
			JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), &offset, &Hit, &mDis, 2.5f, "my");
			if (Hit == TRUE)
				if (mDis < (JDG_LEN + offset))
				{
					if (*player->FlagGetter() == false) player->FlagSetter(true);
					player->FallSetter(0.0f);
					tmpTransMat._42 += (JDG_LEN + offset) - mDis;
				}
			tmpFlg = false;
			if (JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f) == false) tmpFlg = true;
			if (tmpFlg)
			{
				tmpTransMat._42 += 1.0f;
				JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f);
				tmpTransMat._42 -= 1.0f;
			}
		}
		//　ウッド
		for (auto &&p : arr_wd)
		{
			BOOL Hit = FALSE;
			float mDis = 0;
			JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), &offset, &Hit, &mDis, 2.5f, "my");
			if (Hit == TRUE)
				if (mDis < (JDG_LEN + offset))
				{
					if (*player->FlagGetter() == false) player->FlagSetter(true);
					player->FallSetter(0.0f);
					tmpTransMat._42 += (JDG_LEN + offset) - mDis;
				}
			tmpFlg = false;
			if (JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f) == false) tmpFlg = true;
			if (tmpFlg)
			{
				tmpTransMat._42 += 1.0f;
				JudgeManager::JudgeRayPitchRollArrow(&tmpTransMat, player->DirVecGetter(), p->MeshGetter(), p->MatGetter(), 2.0f);
				tmpTransMat._42 -= 1.0f;
			}
		}

		player->MatSetter(nullptr, nullptr, &tmpTransMat);

		////　ストーン
		for (unsigned int i = 0; i < arr_st.size(); i++)
		{
			tmpTransMat = *arr_st[i]->TransMatGetter();

			//　ゴール
			if (1)
			{
				BOOL Hit = FALSE;
				float mDis = 0;
				JudgeManager::JudgeRayPitchRoll(&tmpTransMat, goal->MeshGetter(), goal->MatGetter(), 2.5f);
			}
			//　スイッチ
			for (auto &&p : arr_sw)
			{
				BOOL Hit = FALSE;
				float mDis = 0;
				JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), nullptr, &Hit, &mDis, 2.5f, "my");
				if (Hit == TRUE)
					if (mDis < JDG_LEN)
					{
						if (*arr_st[i]->FlagGetter() == true)
						{
							arr_st[i]->FlagSetter(false);
							if (p->PreparateRotation() == false) return false;
						}
						arr_st[i]->FallSetter(0.0f);
						tmpTransMat._42 += JDG_LEN - mDis;
					}
				JudgeManager::JudgeRayPitchRoll(&tmpTransMat, p->MeshGetter(), p->MatGetter(), 2.5f);
			}
			//　ストーン
			for (auto &&p : arr_st)
			{
				BOOL Hit = FALSE;
				float mDis = 0;
				if (*arr_st[i]->ObjectIDGetter() != *p->ObjectIDGetter())
				{
					BOOL Hit = FALSE;
					float mDis = 0;
					JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), nullptr, &Hit, &mDis, 2.5f, "my");
					if (Hit == TRUE)
						if (mDis < JDG_LEN)
						{
							tmpTransMat._42 += JDG_LEN - mDis;
							if (*arr_st[i]->FlagGetter() == false) arr_st[i]->FlagSetter(true);
							arr_st[i]->FallSetter(0.0f);
						}
					JudgeManager::JudgeRayPitchRoll(&tmpTransMat, p->MeshGetter(), p->MatGetter(), 2.5f);
				}
			}
			//　ガラス
			bool tmpFlg_1 = false;
			for (auto &&p : arr_gl)
			{
				BOOL Hit = FALSE;
				float mDis = 0;
				JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), nullptr, &Hit, &mDis, 2.5f, "my");
				if (Hit == TRUE)
				{
					if (mDis < JDG_LEN)
					{
						float a;
						a = *arr_st[i]->FallGetter();
						if (*arr_st[i]->FallGetter() >= 0.15f) p->FlagSetter(false);

						tmpTransMat._42 += JDG_LEN - mDis;
						if (*arr_st[i]->FlagGetter() == false) arr_st[i]->FlagSetter(true);
						arr_st[i]->FallSetter(0.0f);

						if (tmpFlg) p->FlagSetter(false);
					}
					if (*arr_st[i]->FallGetter() >= 0.5f) tmpFlg_1 = true;
				}
				JudgeManager::JudgeRayPitchRoll(&tmpTransMat, p->MeshGetter(), p->MatGetter(), 2.5f);
			}
			//　ウッド
			for (auto &&p : arr_wd)
			{
				BOOL Hit = FALSE;
				float mDis = 0;
				JudgeManager::JudgeRayYaw(&tmpTransMat, p->MeshGetter(), p->MatGetter(), nullptr, &Hit, &mDis, 2.5f, "my");
				if (Hit == TRUE)
					if (mDis < JDG_LEN)
					{
						if (*arr_st[i]->FlagGetter() == false) arr_st[i]->FlagSetter(true);
						arr_st[i]->FallSetter(0.0f);
						tmpTransMat._42 += JDG_LEN - mDis;
					}
				JudgeManager::JudgeRayPitchRoll(&tmpTransMat, p->MeshGetter(), p->MatGetter(), 2.5f);
			}
			arr_st[i]->MatSetter(nullptr, nullptr, &tmpTransMat);
		}
	}
#pragma endregion

#pragma region 更新処理
	player->Update();
	for (auto &&p : arr_st) p->Update();
#pragma endregion
	return true;
}

void SceneGame::Render2D()
{
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	goal->Render2D();
	lpSprite->End();
}
void SceneGame::Render3D()
{
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	bg->Render3D();
	stgBase->Render3D();

	lpD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	player->Render3D();
	goal->Render3D();

	for (auto && p : arr_sw)	p->Render3D();
	for (auto && p : arr_wd)	p->Render3D();
	for (auto && p : arr_st)	p->Render3D();

	//　透明加工したもの等
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	for (auto && p : arr_gl)	p->Render3D();

	D3DXMATRIX t_Mat, r_Mat;
	D3DXMatrixRotationYawPitchRoll(&r_Mat, 0, D3DXToRadian(30), 0);
	D3DXMatrixTranslation(&t_Mat, camPos.x, camPos.y, camPos.z);
	lpD3DDevice->SetTransform(D3DTS_WORLD, &(r_Mat * camRotMat * t_Mat));

	lpD3DDevice->SetTexture(0, NULL);
	lpD3DDevice->SetFVF(FVF_VERTEX);
	lpD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &jdgVer, sizeof(VERTEX));
}

void SceneGame::SetCamera()
{
#pragma region TPS視点の処理
	camYMat = *crsr->RotYMatSetter();
	camXMat = *crsr->RotXMatSetter();
	camRotMat = camXMat * camYMat;

	D3DXVECTOR3 tmpPos(0.0f, 2.0f, -4.0f);
	D3DXVec3TransformNormal(&camPos, &tmpPos, &camRotMat);

	plyMat = *player->TransMatGetter();
	D3DXVECTOR3 tmpLook(0, 1.8f, 0);
	D3DXVec3TransformCoord(&camLook, &tmpLook, &plyMat);

	camPos += camLook;

#pragma endregion

#pragma region 壁埋め防止処理
	D3DXVECTOR3 tmpVec;
	tmpVec = camPos - camLook;

	float tmpLen;
	tmpLen = D3DXVec3Length(&tmpVec);
	D3DXVec3Normalize(&tmpVec, &tmpVec);

	//　ゴール
	bool hitFlg = false;
	CameraJudge(&tmpLen, &hitFlg, goal->MeshGetter(), goal->MatGetter(), &camLook, &tmpVec);
	//　ウッド
	for (auto &&p : arr_wd) CameraJudge(&tmpLen, &hitFlg, p->MeshGetter(), p->MatGetter(), &camLook, &tmpVec);
	//　ガラス
	for (auto &&p : arr_gl) CameraJudge(&tmpLen, &hitFlg, p->MeshGetter(), p->MatGetter(), &camLook, &tmpVec);
	//　ストーン
	for (auto &&p : arr_st) CameraJudge(&tmpLen, &hitFlg, p->MeshGetter(), p->MatGetter(), &camLook, &tmpVec);
	//　スイッチ
	for (auto &&p : arr_sw) CameraJudge(&tmpLen, &hitFlg, p->MeshGetter(), p->MatGetter(), &camLook, &tmpVec);

	if (hitFlg)
	{
		tmpVec *= tmpLen - 0.1f;
		camPos = camLook + tmpVec;
	}

#pragma endregion


	D3DXMATRIX mView, mProj;
	D3DXMatrixLookAtLH(&mView,
		&camPos,
		&camLook,
		&camHead
	);

	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(60), (float)SCRW / (float)SCRH, 1.0f, 1000.0f);

	lpD3DDevice->SetTransform(D3DTS_VIEW, &mView);
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}

void SceneGame::CameraJudge(float *re_Len, bool *re_Flg, const LPD3DXMESH lpTarMesh, const D3DXMATRIX *tarMat, const D3DXVECTOR3 *hitPos, const D3DXVECTOR3 *dirVec)
{
	float jdgLen;
	D3DXVECTOR3 tarPos = D3DXVECTOR3(
		tarMat->_41,
		tarMat->_42,
		tarMat->_43);
	jdgLen = D3DXVec3Length(&(*hitPos - tarPos));

	if (*re_Len > jdgLen)
	{
		BOOL Hit;
		float mDis;

		Hit = FALSE;
		JudgeManager::JudgeRay(lpTarMesh, tarMat, hitPos, dirVec, &Hit, nullptr, &mDis);
		if (Hit)
		{
			if (*re_Len > mDis)
			{
				*re_Len = mDis;
				*re_Flg = true;
			}
		}
	}
}
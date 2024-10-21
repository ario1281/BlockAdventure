#include"../header/ObjectGoal.h"

template<class T>
inline void SafeRelease(T& lpAdd)
{
	if (lpAdd == nullptr) return;
	lpAdd->Release();
	lpAdd = nullptr;
}

ObjectGoal::ObjectGoal(const D3DXVECTOR3* sPos, char ID, int NumID, char dirID)
{
	objID = ID;
	objNo = NumID;

	Mesh = meshManager.GetMesh("data/mat/mesh/goal.x");

	l_TexLen.w = 242;
	l_TexLen.h = 65;
	logoTex = texManager.GetTexture("data/mat/img/game_clear.png", l_TexLen.w, l_TexLen.h, NULL);
	
	c_TexLen[0].w = 129;
	c_TexLen[0].h = 96;

	c_TexLen[1].w = 173;
	c_TexLen[1].h = 140;

	c_TexLen[2].w = 271;
	c_TexLen[2].h = 170;
	for (int i = 0; i < c_texNum; i++)
	{
		char FileName[32];
		sprintf_s(FileName, sizeof(FileName), "data/mat/img/cracker_%02d.png", i);
		g_crkTex[i] = texManager.GetTexture(FileName, c_TexLen[i / 2].w, c_TexLen[i / 2].h, NULL);
	}
	for (int i = 0; i < c_matNum; i++)
	{
		crkFlg[i] =false;
		D3DXMatrixTranslation(&crkMat[i], 0, SCRH, 0);
	}
	sndManager.GetSound(g_Snd, g_3DSnd, "data/mat/snd/bgm_4.wav");

	if (*taskManager.BGMGetter() != 0)	Vol = (int)(log10f((float)*taskManager.BGMGetter()) * 5000.0f) - 10000;
	else								Vol = -10000;
	g_Snd->SetVolume(Vol);

	const float x_Len = l_TexLen.w / 5.0f;
	for (int i = 0; i < logoNum; i++)
		D3DXMatrixTranslation(&logoMat[i], TEXW(l_TexLen.w) / 2.0f +(float)x_Len * i, TEXH(l_TexLen.h) / 2.0f, 0);

#pragma region obj方向(床の位置)
	switch (dirID)
	{
	case down:
		D3DXMatrixRotationZ(&RotMat, D3DXToRadian(0));
		break;

	case left:
		D3DXMatrixRotationZ(&RotMat, D3DXToRadian(-90));
		break;

	case rear:
		D3DXMatrixRotationX(&RotMat, D3DXToRadian(90));
		break;

	case front:
		D3DXMatrixRotationX(&RotMat, D3DXToRadian(-90));
		break;

	case right:
		D3DXMatrixRotationZ(&RotMat, D3DXToRadian(90));
		break;

	case up:
		D3DXMatrixRotationX(&RotMat, D3DXToRadian(180));
		break;

	default:
		D3DXMatrixRotationZ(&RotMat, D3DXToRadian(0));
		break;
	}
#pragma endregion

	D3DXMatrixTranslation(&TransMat, sPos->x, sPos->y, sPos->z);

	Mat = RotMat * TransMat;

	g_Flg = false;
	g_sndFlg = false;
}
ObjectGoal::~ObjectGoal()
{
	//　テクスチャ
	SafeRelease(logoTex);
	for (auto &&p : g_crkTex) SafeRelease(p);
	for (auto &&p : s_crkTex) if(p != nullptr) SafeRelease(p);

	//　サウンド
	SafeRelease(g_Snd);
	SafeRelease(g_3DSnd);
}

bool ObjectGoal::Update()
{
	if (g_Flg)
	{
		if (g_sndFlg == false)
		{
			g_sndFlg = true;
			g_Snd->Play(0, 0, 0);
		}

		DWORD sndFlg;
		g_Snd->GetStatus(&sndFlg);
		if ((sndFlg & DSBSTATUS_PLAYING) == 0) g_Snd->Stop();

		logoSym = (1 + cos(D3DXToRadian(180 + triAng / 2.0f))) / (0.5f + zeroCnt * 0.5f);
		triAng++;
		if (triAng > 1080) triAng = 1080;
		if (logoSym == 0.0f) zeroCnt++;

		////　　　紙吹雪　　　////
		//　出現をする
		for (int i = 0; i < c_matNum; i++)
		{
			if (rand() % 1250 < 1)
			{
				if (crkFlg[i] == false)
				{
					crkFlg[i] = true;

					rndTex[i] = rand() % (c_texNum - 1);
					s_crkTex[i] = g_crkTex[rndTex[i]];

					D3DXMatrixTranslation(&crkMat[i],
						(float)(rand() % (SCRW - c_TexLen[rndTex[i] / 2].w)),
						(float)(0 - c_TexLen[rndTex[i] / 2].h),
						(float)0);

					break;
				}
			}
		}

		//　出現してる
		for (int i = 0; i < c_matNum; i++)
		{
			if (crkFlg[i])
			{
				crkMat[i]._42 += 0.5f;

				if (c_cnt[i] <= 0 && rand() % 100 < 25)
				{
					rndTex[i] = rand() % (c_texNum - 1);
					s_crkTex[i] = g_crkTex[rndTex[i]];
					c_cnt[i] = 60 * 2;
				}
				c_cnt[i]--;

				if (crkMat[i]._42 > SCRH)
				{
					crkFlg[i] = false;
				}
			}
		
		}
	}
	return true;
}
void ObjectGoal::Render2D()
{
	if (g_Flg)
	{
		float Rect_x = l_TexLen.w / 5.0f;
		for (int i = 0; i < logoNum; i++)
		{
			RECT rcClear = { (LONG)(Rect_x * i),0,(LONG)(Rect_x * (i + 1)),(LONG)l_TexLen.w };

			D3DXMATRIX tmpMat;
			float	offset_x = 50.0f,
				offset_y = 100.0f;
			D3DXMatrixTranslation(&tmpMat, -(float)l_TexLen.w / 2.0f + offset_x, -(float)l_TexLen.h / 2.0f + offset_y, 0);
			tmpMat._41 += l_TexLen.w / 5.0f * ((logoNum - 1) - i);

			D3DXMATRIX sclMat;
			D3DXMatrixScaling(&sclMat, logoSym, logoSym, logoSym);
			sclMat = sclMat * tmpMat;
			D3DXMatrixInverse(&tmpMat, NULL, &tmpMat);
			sclMat = tmpMat * sclMat;
			Draw2D(&(sclMat * logoMat[i]), logoTex, &rcClear, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		for (int i = 0; i < c_matNum; i++)
		{
			if (crkFlg[i])
			{
				RECT rcCrack = { 0,0,(LONG)c_TexLen[rndTex[i] / 2].w,(LONG)c_TexLen[rndTex[i] / 2].h };
				Draw2D(&crkMat[i], s_crkTex[i], &rcCrack, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
}
void ObjectGoal::Render3D()
{
	lpD3DDevice->SetTransform(D3DTS_WORLD, &Mat);
	DrawMesh(&Mesh);
}
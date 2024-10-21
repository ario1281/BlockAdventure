#include"../header/SoundManager.h"

SoundManager::~SoundManager()
{
	AllDelete();
	CoUninitialize();
}

void SoundManager::GetSound(LPDIRECTSOUNDBUFFER8 &p_snd, LPDIRECTSOUND3DBUFFER8 &p_snd3D, std::string FileName)
{
	if (sndList.find(FileName) == sndList.end())
	{
		LoadWAVE3D(p_snd, p_snd3D, &FileName[0]);

		LPDIRECTSOUNDBUFFER lpTmpSnd;
		lpDSound->DuplicateSoundBuffer(p_snd, &lpTmpSnd);
		lpTmpSnd->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&p_snd);
		p_snd->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&p_snd3D);
		p_snd3D->SetMode(DS3DMODE_DISABLE, DS3D_IMMEDIATE);
		lpTmpSnd->Release();

		sndList[FileName].first = p_snd;
		sndList[FileName].second = p_snd3D;
	}
	else
	{
		p_snd = sndList[FileName].first;
		p_snd3D = sndList[FileName].second;
	}
}

void SoundManager::AllDelete()
{
	//for (auto itr = sndList.begin(); itr != sndList.end(); ++itr)
	//{
	//	itr->second.first->Release();
	//	itr->second.second->Release();
	//}
	SoundManager::sndList.clear();
}

int SoundManager::SoundVolume(const int g_Vol, double agust)
{
	int s_Vol;
	if (g_Vol != 0)	 s_Vol = (int)(log10f((float)g_Vol) * 5000.0f) - 10000;
	else								 s_Vol = -10000;
	return (int)(s_Vol * agust / 100.0f);
}


void SoundManager::LoadWAVE3D(LPDIRECTSOUNDBUFFER8 &pDSData, LPDIRECTSOUND3DBUFFER8 &pDSData3D, char *fname)
{
	HRESULT hr;

	CWaveSoundRead WaveFile;
	WaveFile.Open(fname);

	DSBUFFERDESC dsbdesc;
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);

	dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRL3D |
		DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbdesc.dwBufferBytes = WaveFile.m_ckIn.cksize;
	dsbdesc.lpwfxFormat = WaveFile.m_pwfx;

	dsbdesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;

	LPDIRECTSOUNDBUFFER pDSTmp;
	lpDSound->CreateSoundBuffer(&dsbdesc, &pDSTmp, NULL);
	pDSTmp->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pDSData);
	pDSTmp->Release();

	LPVOID lpvPtr1;
	DWORD dwBytes1;
	LPVOID lpvPtr2;
	DWORD dwBytes2;

	hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);

	if (DSERR_BUFFERLOST == hr)
	{
		pDSData->Restore();
		hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
	}
	if (SUCCEEDED(hr))
	{
		UINT rsize;
		WaveFile.Read(dwBytes1, (LPBYTE)lpvPtr1, &rsize);
		if (0 != dwBytes2)
			WaveFile.Read(dwBytes2, (LPBYTE)lpvPtr2, &rsize);

		hr = pDSData->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
	}
	pDSData->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&pDSData3D);
}
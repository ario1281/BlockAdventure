#pragma once
#include<string>
#include <utility>
#include<map>

#include"define.h"

// vol 0~100
#define SND_VOL_PER( volume ) (LONG)(-10000 + (10000 * ((volume) / 100.0f)))

class SoundManager
{
public:
	~SoundManager();
	static int SoundVolume(const int g_Vol, double agust);
	void GetSound(LPDIRECTSOUNDBUFFER8 &p_snd, LPDIRECTSOUND3DBUFFER8 &p_snd3D, std::string FileName);
	void AllDelete();

private:
	void LoadWAVE3D(LPDIRECTSOUNDBUFFER8 &pDSData, LPDIRECTSOUND3DBUFFER8 &pDSData3D, char *fname);
	std::map<std::string, std::pair<LPDIRECTSOUNDBUFFER8, LPDIRECTSOUND3DBUFFER8>>sndList;
};
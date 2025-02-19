#pragma once
/*===================================================================
	DirectXAudio(DirectSound)管理
===================================================================*/
class DirectXAudio
{
public:

	#pragma region 取得系

	LPGUID						GetGuid()			{ return m_lpGuid; }
	LPDIRECTSOUND8				GetDirectSound()	{ return m_lpDSound; }
	LPDIRECTSOUNDBUFFER			GetSoundBuffer()	{ return m_lpDSBuffer; }
	LPDIRECTSOUND3DLISTENER8	GetListener()		{ return m_lpDSListen; }

	#pragma endregion

	bool Init(HWND _hWnd, std::string &errMsg);
	void Release() {
		SafeRelease(m_lpDSBuffer);
		SafeRelease(m_lpDSound);
		//SafeRelease(m_lpDSListen);
	}

	HRESULT SetLisnerLookAtLH(Vector3 &s_Eye, Vector3 &s_At, Vector3 &s_Up);
	HRESULT SetLisnerLookAtRH(Vector3 &s_Eye, Vector3 &s_At, Vector3 &s_Up);

	~DirectXAudio() {
		Release();
	}


private:
	//　構造
	//　Data1	…　NULL
	//　Data2	…　DSDEVID_DefaultPlayback
	//　Data3	…　DSDEVID_DefaultVoicePlayback
	//　Data4	…　DirectSoundEnumerate関数で取得したGUID
	LPGUID						m_lpGuid		= nullptr;
	//　インターフェイス
	LPDIRECTSOUND8				m_lpDSound		= nullptr;
	//　サウンドバッファ
	LPDIRECTSOUNDBUFFER			m_lpDSBuffer	= nullptr;
	//　リスナー
	LPDIRECTSOUND3DLISTENER8	m_lpDSListen	= NULL;

#pragma region シングルトン
public:
	static DirectXAudio& GetInstance() {
		static DirectXAudio Instance;
		return Instance;
	}
private:
	DirectXAudio() {}

#pragma endregion

};

#define DX_AUDIO DirectXAudio::GetInstance()


/*===================================================================
	DS,DS3D読み込み / データ保管
===================================================================*/
class DirectSoundManager
{
public:
	// 読み込み
	// ・filename		…　ファイル名
	// ・戻り値			…　成功:true
	virtual bool Load(const std::string &name);

	/*------------------------------------------------
		再生されているか
	------------------------------------------------*/
	bool IsPlaying() {
		DWORD flg;
		m_lpSDBuffer->GetStatus(&flg);
		return (flg & DSBSTATUS_PLAYING) ? false : true;
	}

	//　再生
	void Play(bool loop = false) {
		if (loop) { m_lpSDBuffer->Play(0, 0, DSBPLAY_LOOPING); }
		else { m_lpSDBuffer->Play(0, 0, 0); }
	}
	//　停止
	void Stop() {
		m_lpSDBuffer->Stop();
		m_lpSDBuffer->SetCurrentPosition(0);
	}

	void Release() {
		SafeRelease(m_lpSDBuffer);
		SafeRelease(m_lpSD3DBuffer);
	}

	~DirectSoundManager() {
		Release();
	}

protected:
	bool LoadWAVE3D
	(
		const std::string		&name,			//　ファイル名
		LPDIRECTSOUNDBUFFER8	&lpDSBuff,		//　バッファ
		LPDIRECTSOUND3DBUFFER8	&lpDS3DBuff		//　3Dバッファ
	);

	LPDIRECTSOUNDBUFFER8	m_lpSDBuffer;
	LPDIRECTSOUND3DBUFFER8	m_lpSD3DBuffer;
};
class DirectSound3DManager : public DirectSoundManager
{
public:
	// 読み込み
	// ・filename		…　ファイル名
	// ・戻り値			…　成功:true
	bool Load(const std::string &name)override;

	~DirectSound3DManager() {
		Release();
	}

	//　セットポジション
	HRESULT SetPosition(float x, float y, float z) {
		return m_lpSD3DBuffer->SetPosition(x, y, z, DS3D_IMMEDIATE);
	}
	HRESULT SetPosition(Vector3 v) {
		return m_lpSD3DBuffer->SetPosition(v.x, v.y, v.z, DS3D_IMMEDIATE);
	}

	bool SetListener(Matrix &s_CamMat);

};

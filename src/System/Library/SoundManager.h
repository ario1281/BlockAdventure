#pragma once
/*===================================================================
	DirectXAudio(DirectSound)�Ǘ�
===================================================================*/
class DirectXAudio
{
public:

	#pragma region �擾�n

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
	//�@�\��
	//�@Data1	�c�@NULL
	//�@Data2	�c�@DSDEVID_DefaultPlayback
	//�@Data3	�c�@DSDEVID_DefaultVoicePlayback
	//�@Data4	�c�@DirectSoundEnumerate�֐��Ŏ擾����GUID
	LPGUID						m_lpGuid		= nullptr;
	//�@�C���^�[�t�F�C�X
	LPDIRECTSOUND8				m_lpDSound		= nullptr;
	//�@�T�E���h�o�b�t�@
	LPDIRECTSOUNDBUFFER			m_lpDSBuffer	= nullptr;
	//�@���X�i�[
	LPDIRECTSOUND3DLISTENER8	m_lpDSListen	= NULL;

#pragma region �V���O���g��
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
	DS,DS3D�ǂݍ��� / �f�[�^�ۊ�
===================================================================*/
class DirectSoundManager
{
public:
	// �ǂݍ���
	// �Efilename		�c�@�t�@�C����
	// �E�߂�l			�c�@����:true
	virtual bool Load(const std::string &name);

	/*------------------------------------------------
		�Đ�����Ă��邩
	------------------------------------------------*/
	bool IsPlaying() {
		DWORD flg;
		m_lpSDBuffer->GetStatus(&flg);
		return (flg & DSBSTATUS_PLAYING) ? false : true;
	}

	//�@�Đ�
	void Play(bool loop = false) {
		if (loop) { m_lpSDBuffer->Play(0, 0, DSBPLAY_LOOPING); }
		else { m_lpSDBuffer->Play(0, 0, 0); }
	}
	//�@��~
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
		const std::string		&name,			//�@�t�@�C����
		LPDIRECTSOUNDBUFFER8	&lpDSBuff,		//�@�o�b�t�@
		LPDIRECTSOUND3DBUFFER8	&lpDS3DBuff		//�@3D�o�b�t�@
	);

	LPDIRECTSOUNDBUFFER8	m_lpSDBuffer;
	LPDIRECTSOUND3DBUFFER8	m_lpSD3DBuffer;
};
class DirectSound3DManager : public DirectSoundManager
{
public:
	// �ǂݍ���
	// �Efilename		�c�@�t�@�C����
	// �E�߂�l			�c�@����:true
	bool Load(const std::string &name)override;

	~DirectSound3DManager() {
		Release();
	}

	//�@�Z�b�g�|�W�V����
	HRESULT SetPosition(float x, float y, float z) {
		return m_lpSD3DBuffer->SetPosition(x, y, z, DS3D_IMMEDIATE);
	}
	HRESULT SetPosition(Vector3 v) {
		return m_lpSD3DBuffer->SetPosition(v.x, v.y, v.z, DS3D_IMMEDIATE);
	}

	bool SetListener(Matrix &s_CamMat);

};

#include"define.h"
#include"wavread.h"
#include"SoundManager.h"

//�@�T�E���h�t�@�C���̓ǂݍ���
bool DirectSoundManager::Load(const std::string & name)
{
	if (!LoadWAVE3D(name, m_lpSDBuffer, m_lpSD3DBuffer)) { return false; }

	LPDIRECTSOUNDBUFFER lpTmpSDBuffer;
	DX_AUDIO.GetDirectSound()->DuplicateSoundBuffer(m_lpSDBuffer, &lpTmpSDBuffer);
	lpTmpSDBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_lpSD3DBuffer);
	m_lpSD3DBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&m_lpSD3DBuffer);
	m_lpSD3DBuffer->SetMode(DS3DMODE_DISABLE, DS3D_IMMEDIATE);
	SafeRelease(lpTmpSDBuffer);

	return true;
}
bool DirectSound3DManager::Load(const std::string & name)
{
	if (!LoadWAVE3D(name, m_lpSDBuffer, m_lpSD3DBuffer)) { return false; }

	LPDIRECTSOUNDBUFFER lpTmpSDBuffer;
	DX_AUDIO.GetDirectSound()->DuplicateSoundBuffer(m_lpSDBuffer, &lpTmpSDBuffer);
	lpTmpSDBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_lpSD3DBuffer);
	m_lpSD3DBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&m_lpSD3DBuffer);
	m_lpSD3DBuffer->SetMode(DS3DMODE_NORMAL, DS3D_IMMEDIATE);
	SafeRelease(lpTmpSDBuffer);

	return true;
}

bool DirectSoundManager::LoadWAVE3D(const std::string &name,
	LPDIRECTSOUNDBUFFER8 &_lpDSBuffer, LPDIRECTSOUND3DBUFFER8 &_lpDS3DBuffer)
{
	CWaveSoundRead wavFile;
	if (FAILED(wavFile.Open(const_cast<char*>(name.c_str())))) { return false; }
	//wavFile.Open(const_cast<char*>(name.c_str()));

	//�@�Z�J���_���E�o�b�t�@���쐬����
	//�@DSBUFFERDESC�\���̂�ݒ�
	DSBUFFERDESC dsbdesc;
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);

	//�@(DSBCAPS_CTRL3D = 3D�T�E���h���g�p)
	dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRL3D |
		DSBCAPS_CTRLVOLUME | /*DSBCAPS_CTRLPAN |*/ DSBCAPS_CTRLFREQUENCY;
	dsbdesc.dwBufferBytes = wavFile.m_ckIn.cksize;
	dsbdesc.lpwfxFormat = wavFile.m_pwfx;

	//�@3D�T�E���h��HEL�A���S���Y����I��
	dsbdesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;

	//�@�o�b�t�@�����
	LPDIRECTSOUNDBUFFER lpDSBuffer;
	DX_AUDIO.GetDirectSound()->CreateSoundBuffer(&dsbdesc, &lpDSBuffer, NULL);
	lpDSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&_lpDSBuffer);
	lpDSBuffer->Release();

	//�@�Z�J���_���E�o�b�t�@��Wave�f�[�^����������
	LPVOID	lpvPtr1;	//�@�ŏ��̃u���b�N�̃|�C���^
	DWORD	dwBytes1;	//�@�ŏ��̃u���b�N�̃T�C�Y
	LPVOID	lpvPtr2;	//�@2�Ԗڂ̃u���b�N�̃|�C���^
	DWORD	dwBytes2;	//�@2�Ԗڂ̃u���b�N�̃T�C�Y
	auto hr = _lpDSBuffer->Lock(0, wavFile.m_ckIn.cksize,
		&lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);

	//�@�o�b�t�@����
	if (hr == DSERR_BUFFERLOST) {
		_lpDSBuffer->Restore();
		hr = _lpDSBuffer->Lock(0, wavFile.m_ckIn.cksize, &lpvPtr1,
			&dwBytes1, &lpvPtr2, &dwBytes2, 0);
	}
	if (SUCCEEDED(hr)) {
		//�@��������
		//�@�f�[�^�R�s�[
		UINT rsize;
		wavFile.Read(dwBytes1, (LPBYTE)lpvPtr1, &rsize);
		if (0 != dwBytes2) wavFile.Read(dwBytes2, (LPBYTE)lpvPtr2, &rsize);

		//�@�������݂��I������炷����Unlock����D
		hr = _lpDSBuffer->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
	}

	_lpDSBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&_lpDS3DBuffer);
	return true;
}

bool DirectSound3DManager::SetListener(Matrix & s_camMat)
{
	//�@���X�i�[�̍��W(eye)
	Vector3 lisPos(s_camMat._41, s_camMat._42, s_camMat._43);
	//�@���X�i�[��Look(at)�x�N�g��
	Vector3 lisLook(s_camMat._31, s_camMat._32, s_camMat._33);
	//�@���X�i�[��Head(up)�x�N�g��
	Vector3 lisHead;
	D3DXVec3Cross(&lisHead, &lisLook, &D3DXVECTOR3(s_camMat._11, s_camMat._12, s_camMat._13));
	lisHead.Normalize();

	DX_AUDIO.SetLisnerLookAtRH(lisPos, lisLook, lisHead);

	return true;
}





//�@DirectSound�̏���
bool DirectXAudio::Init(HWND _hWnd, std::string &errMsg)
{
	//�@DirectSound�̐���
	if (FAILED(DirectSoundCreate8(m_lpGuid, &m_lpDSound, nullptr)))
	{
		errMsg = "�T�E���h�I�u�W�F�N�g�쐬���s\n";
		return false;
	}

	//�@�������x���ݒ�̎���
	if (FAILED(m_lpDSound->SetCooperativeLevel(_hWnd, DSSCL_PRIORITY)))
	{
		errMsg = "�������x���ݒ莸�s\n";
		return false;
	}

	//�@�v���C�}���T�E���h�o�b�t�@�̍쐬
	DSBUFFERDESC dsbdesc;
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize			= sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags			= DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes	= 0;
	dsbdesc.guid3DAlgorithm	= DS3DALG_DEFAULT;
	dsbdesc.lpwfxFormat		= nullptr;

	if (FAILED(m_lpDSound->CreateSoundBuffer(&dsbdesc, &m_lpDSBuffer, NULL)))
	{
		errMsg = "�v���C�}���T�E���h�o�b�t�@�쐬���s\n";
		return false;
	}

	//�@�v���C�}���o�b�t�@�̃X�e�[�^�X������
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.wFormatTag			= WAVE_FORMAT_PCM;
	wfex.nChannels			= 2;
	wfex.nSamplesPerSec		= 44100;
	wfex.nBlockAlign		= 4;
	wfex.nAvgBytesPerSec	= wfex.nSamplesPerSec * wfex.nBlockAlign;
	wfex.wBitsPerSample		= 16;
	if (FAILED(m_lpDSBuffer->SetFormat(&wfex)))
	{
		errMsg = "�v���C�}���o�b�t�@�̃X�e�[�^�X���s\n";
		return false;
	}

	if (FAILED(m_lpDSBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_lpDSListen)))
	{
		errMsg = "3D���X�i�[�̎擾���s\n";
		return false;
	}
	if (FAILED(m_lpDSListen->SetRolloffFactor(0.5f, DS3D_IMMEDIATE)))
	{
		errMsg = "���[���I�t�ݒ莸�s\n";
		return false;
	}
	
	return true;
}


//�@eye���}�C�i�X�l�ɂ��Čv�Z
HRESULT DirectXAudio::SetLisnerLookAtLH(
	Vector3 &s_Eye, Vector3 &s_At, Vector3 &s_Up)
{
	m_lpDSListen->SetPosition(
		-s_Eye.x, -s_Eye.y, -s_Eye.z,
		DS3D_IMMEDIATE);

	m_lpDSListen->SetOrientation(
		s_At.x, s_At.y, s_At.z,
		s_Up.x, s_Up.y, s_Up.z,
		DS3D_IMMEDIATE);
	return E_NOTIMPL;
}
//�@eye�����̂܂܂Ōv�Z
HRESULT DirectXAudio::SetLisnerLookAtRH(
	Vector3 &s_Eye, Vector3 &s_At, Vector3 &s_Up)
{
	m_lpDSListen->SetPosition(
		s_Eye.x, s_Eye.y, s_Eye.z,
		DS3D_IMMEDIATE);

	m_lpDSListen->SetOrientation(
		s_At.x, s_At.y, s_At.z,
		s_Up.x, s_Up.y, s_Up.z,
		DS3D_IMMEDIATE);
	return E_NOTIMPL;
}

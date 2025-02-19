#include"define.h"
#include"MeshManager.h"

bool MeshManager::LoadXFile(const std::string & filename)
{
	Release();

	#pragma region �t�@�C�����L��
	// �p�X�A�t�@�C��������
	std::string	Path;
	char		szDir[_MAX_PATH];		// �t�H���_
	char		szFname[_MAX_FNAME];	// �t�@�C����(�g���q������)
	char		szExt[_MAX_EXT];		// �g���q
	_splitpath_s(
		filename.c_str(), nullptr, 0,
		szDir, sizeof(szDir),
		szFname, sizeof(szFname),
		szExt, sizeof(szExt));
	Path = szDir;


	m_fileName = szFname;
	m_fileName += szExt;
	#pragma endregion

	// X�t�@�C���ǂݍ���
	HRESULT			hr;
	LPD3DXBUFFER	lpD3DXBuffer;
	DWORD			matNum;

	hr = D3DXLoadMeshFromX(filename.c_str(),
		D3DXMESH_SYSTEMMEM,
		DEVICE,
		nullptr,
		&lpD3DXBuffer,		//�@�}�e���A���f�[�^���i�[���邽�߂̕ϐ����w��
		nullptr,
		&matNum,			//�@�}�e���A�������i�[���邽�߂̕ϐ����w��
		&m_lpMesh);			//�@���b�V���f�[�^���i�[���邽�߂̕ϐ����w��
	if (FAILED(hr)) { return false; }

	// ���b�V���̃}�e���A���̑����Ɋ�Â��āA�}�e���A���z����쐬
	m_materials.resize(matNum);

	// lpD3DXBuffer ����A������e�N�X�`���[�̏���ǂݎ��
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)lpD3DXBuffer->GetBufferPointer();

	// �}�e���A���R�s�[�A�e�N�X�`���ǂݍ���
	std::string strTexFile;
	for (DWORD i = 0; i < matNum; i++) {
		// �ގ�
		m_materials[i].d3dMat = d3dxMaterials[i].MatD3D;				// �����̃R�s�[
		m_materials[i].d3dMat.Ambient = m_materials[i].d3dMat.Diffuse;	// Ambient��Diffuse�̃R�s�[���g�p
														// �e�N�X�`���� Path + �t�@�C����
		strTexFile = Path;
		if (d3dxMaterials[i].pTextureFilename) {
			strTexFile += d3dxMaterials[i].pTextureFilename;
		}
		// �e�N�X�`���ǂݍ���
		m_materials[i].pTex = ResFac.GetTexture(strTexFile);
	}
	// ��Ɨp�}�e���A�� �o�b�t�@�J��
	SafeRelease(lpD3DXBuffer);

	//�@�N���[�����b�V���̍쐬
	LPD3DXMESH lpMesh = nullptr;
	m_lpMesh->CloneMeshFVF(
		D3DXMESH_NPATCHES | D3DXMESH_SYSTEMMEM,
		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
		DEVICE, &lpMesh);

	//�@�I���W�i�����b�V���̍폜
	SafeRelease(m_lpMesh);
	m_lpMesh = lpMesh;

	return true;
}

void MeshManager::Release()
{
	SafeRelease(m_lpMesh);
	SafeClear(m_materials);
	m_fileName = "";
}

void MeshManager::Draw()
{
	if (m_lpMesh == nullptr) return;

	for (UINT i = 0; i < m_materials.size(); i++)
	{
		DEVICE->SetMaterial(&m_materials[i].d3dMat);

		auto &tex = m_materials[i].pTex;
		DEVICE->SetTexture(0, tex ? tex->GetTex() : nullptr);

		m_lpMesh->DrawSubset(i);
	}
}
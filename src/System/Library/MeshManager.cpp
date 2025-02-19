#include"define.h"
#include"MeshManager.h"

bool MeshManager::LoadXFile(const std::string & filename)
{
	Release();

	#pragma region ファイル名記憶
	// パス、ファイル名分解
	std::string	Path;
	char		szDir[_MAX_PATH];		// フォルダ
	char		szFname[_MAX_FNAME];	// ファイル名(拡張子を除く)
	char		szExt[_MAX_EXT];		// 拡張子
	_splitpath_s(
		filename.c_str(), nullptr, 0,
		szDir, sizeof(szDir),
		szFname, sizeof(szFname),
		szExt, sizeof(szExt));
	Path = szDir;


	m_fileName = szFname;
	m_fileName += szExt;
	#pragma endregion

	// Xファイル読み込み
	HRESULT			hr;
	LPD3DXBUFFER	lpD3DXBuffer;
	DWORD			matNum;

	hr = D3DXLoadMeshFromX(filename.c_str(),
		D3DXMESH_SYSTEMMEM,
		DEVICE,
		nullptr,
		&lpD3DXBuffer,		//　マテリアルデータを格納するための変数を指定
		nullptr,
		&matNum,			//　マテリアル数を格納するための変数を指定
		&m_lpMesh);			//　メッシュデータを格納するための変数を指定
	if (FAILED(hr)) { return false; }

	// メッシュのマテリアルの総数に基づいて、マテリアル配列を作成
	m_materials.resize(matNum);

	// lpD3DXBuffer から、質感やテクスチャーの情報を読み取る
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)lpD3DXBuffer->GetBufferPointer();

	// マテリアルコピー、テクスチャ読み込み
	std::string strTexFile;
	for (DWORD i = 0; i < matNum; i++) {
		// 材質
		m_materials[i].d3dMat = d3dxMaterials[i].MatD3D;				// 質感のコピー
		m_materials[i].d3dMat.Ambient = m_materials[i].d3dMat.Diffuse;	// AmbientはDiffuseのコピーを使用
														// テクスチャ名 Path + ファイル名
		strTexFile = Path;
		if (d3dxMaterials[i].pTextureFilename) {
			strTexFile += d3dxMaterials[i].pTextureFilename;
		}
		// テクスチャ読み込み
		m_materials[i].pTex = ResFac.GetTexture(strTexFile);
	}
	// 作業用マテリアル バッファ開放
	SafeRelease(lpD3DXBuffer);

	//　クローンメッシュの作成
	LPD3DXMESH lpMesh = nullptr;
	m_lpMesh->CloneMeshFVF(
		D3DXMESH_NPATCHES | D3DXMESH_SYSTEMMEM,
		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
		DEVICE, &lpMesh);

	//　オリジナルメッシュの削除
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
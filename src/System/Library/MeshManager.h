#pragma once

class MeshManager
{
public:
	bool LoadXFile(const std::string& filename);

	void Release();

	void Draw();

	~MeshManager() {
		Release();
	}

	const LPD3DXMESH	GetMesh()			{ return m_lpMesh; }
	const size_t		GetMaterialSize()	{ return m_materials.size(); }

private:
	LPD3DXMESH				m_lpMesh	= nullptr;

	struct Material
	{
		D3DMATERIAL9 d3dMat;				//�@�ގ�
		shared_ptr<TextureManager> pTex;	//�@�摜
	};
	std::vector<Material>	m_materials;
	std::string				m_fileName;
};
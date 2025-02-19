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
		D3DMATERIAL9 d3dMat;				//Å@çﬁéø
		shared_ptr<TextureManager> pTex;	//Å@âÊëú
	};
	std::vector<Material>	m_materials;
	std::string				m_fileName;
};
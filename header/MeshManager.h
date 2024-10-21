#pragma once
#include<map>
#include"define.h"

struct CLONEVERTEX
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 Tex;
};
struct XFILE;

class MeshManager
{
public:
	~MeshManager();
	XFILE GetMesh(std::string FileName);
	void AllDelete();

private:
	void LoadMesh(struct XFILE* XFile, const char FName[]);
	void ReleaseMesh(struct XFILE *XFile);
	
	std::map<std::string, XFILE>MeshList;
};
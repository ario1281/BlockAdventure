#include"define.h"
#include"ResourceFactory.h"

//　モデル
shared_ptr<MeshManager> ResourceFactory::GetModel(const std::string &filename)
{
	if (m_mapModel.find(filename) != m_mapModel.end())
		return m_mapModel[filename];

	//　モデルの読み込み
	auto newModel = std::make_shared<MeshManager>();
	if (!newModel->LoadXFile(filename)) return nullptr;

	m_mapModel[filename] = newModel;
	return newModel;
}
//　テクスチャ
shared_ptr<TextureManager> ResourceFactory::GetTexture(const std::string &filename)
{
	if (m_mapTexture.find(filename) != m_mapTexture.end())
		return m_mapTexture[filename];

	//　テクスチャの読み込み
	auto newTex = std::make_shared<TextureManager>();
	if (!newTex->Load(filename)) return nullptr;

	m_mapTexture[filename] = newTex;
	return newTex;
}
//　サウンド
shared_ptr<DirectSoundManager> ResourceFactory::GetSound(const std::string &filename)
{
	if (m_mapSound.find(filename) != m_mapSound.end())
		return m_mapSound[filename];

	//　サウンドの読み込み
	auto newSound = std::make_shared<DirectSoundManager>();
	if (!newSound->Load(filename)) return nullptr;

	m_mapSound[filename] = newSound;
	return newSound;
}

//　Jsonの各関数
//json11::Json
//ResourceFactory::GetJson(const std::string & filename)
//{
//	if (m_mapJson.find(filename) != m_mapJson.end())
//		return m_mapJson[filename];
//
//	auto json = LoadJson(filename);
//	if (json == nullptr) return nullptr;
//
//	m_mapJson[filename] = json;
//	return json;
//}
//json11::Json
//ResourceFactory::MargeJson(const json11::Json &src, const json11::Json& overwrite) const
//{
//	json11::Json::object m_dat = src.object_items();
//
//	for (auto&& ins : overwrite.object_items())
//		m_dat[ins.first] = ins.second;
//
//	return m_dat;
//}
//json11::Json
//ResourceFactory::LoadJson(const std::string & name)
//{
//	std::ifstream ifs(name);
//	if (!ifs) return false;
//
//	std::string strJson(
//		(std::istreambuf_iterator<char>(ifs)),
//		std::istreambuf_iterator<char>());
//
//	std::string err;
//	json11::Json l_objJson = json11::Json::parse(strJson, err);
//	if (err.size() > 0) return nullptr;
//	return l_objJson;
//}

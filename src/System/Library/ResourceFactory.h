#pragma once

class ResourceFactory
{
public:
	void Release() {
		SafeClear(m_mapJson);
		SafeClear(m_mapModel);
		SafeClear(m_mapTexture);
		SafeClear(m_mapSound);
	}

	#pragma region 取得系

	shared_ptr<MeshManager> GetModel(const std::string& filename);
	shared_ptr<TextureManager> GetTexture(const std::string& filename);
	shared_ptr<DirectSoundManager> GetSound(const std::string& filename);

	//json11::Json GetJson(const std::string& name);

	#pragma endregion

	//json11::Json MargeJson(const json11::Json &src, const json11::Json& overwrite) const;

private:
	//　ロード
	//json11::Json LoadJson(const std::string &name);

	//　管理マップ
	std::unordered_map<std::string, shared_ptr<MeshManager>>		m_mapModel;
	std::unordered_map<std::string, shared_ptr<TextureManager>>		m_mapTexture;
	std::unordered_map<std::string, json11::Json>					m_mapJson;
	std::unordered_map<std::string, shared_ptr<DirectSoundManager>>		m_mapSound;

public:
	static ResourceFactory& GetInstance() {
		static ResourceFactory Instance;
		return Instance;
	}

private:
	ResourceFactory() {}
};

#define ResFac ResourceFactory::GetInstance()
#pragma once

class GameObject
{
public:
	GameObject()	{ OutputDebugString("GameObjectが確保"); };
	~GameObject()	{ OutputDebugString("GameObjectが解放"); };

	#pragma region 取得系

	const LPD3DXMESH GetModel() { return m_spModel.get()->GetMesh(); }
	const Matrix& GetWorldMatrix() { return m_mWorld; }
	inline const std::string& GetTag() const { return m_tag; }
	inline const std::string& GetName() const { return m_name; }

	#pragma endregion

	virtual void Deserialize(json11::Json& jObj);

	virtual bool Update(const std::list<shared_ptr<GameObject>> *obj = nullptr) { return false; }
	virtual void Draw2D();
	virtual void Draw3D();

	virtual void ImGui() {
		ImGui::Begin((m_name + " class").c_str());
		ImGui::Text("None");
		ImGui::End();
	};

	void SetModel(const std::string &name);


	shared_ptr<GameObject> Instantiate(const std::string &className);

protected:
	//　オブジェクト情報
	shared_ptr<MeshManager>		m_spModel	= nullptr;
	shared_ptr<TextureManager>	m_spTex		= nullptr;

	Matrix						m_mWorld;
	std::string					m_tag		= "";
	std::string					m_name		= "GameObject";

	// Imgui
	bool					m_Imgui		= false;
};
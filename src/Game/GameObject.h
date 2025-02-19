#pragma once

class GameObject
{
public:
	GameObject()	{ OutputDebugString("GameObject���m��"); };
	~GameObject()	{ OutputDebugString("GameObject�����"); };

	#pragma region �擾�n

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
	//�@�I�u�W�F�N�g���
	shared_ptr<MeshManager>		m_spModel	= nullptr;
	shared_ptr<TextureManager>	m_spTex		= nullptr;

	Matrix						m_mWorld;
	std::string					m_tag		= "";
	std::string					m_name		= "GameObject";

	// Imgui
	bool					m_Imgui		= false;
};
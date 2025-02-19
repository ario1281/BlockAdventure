#include "GameObject.h"

void GameObject::Draw2D()
{
	if (!m_spTex) { return; }

	SPRITE.DrawTex(m_mWorld, m_spTex.get());
}

void GameObject::Draw3D()
{
	if (!m_spModel) { return; }

	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

void GameObject::SetModel(const std::string &name)
{
	m_spModel = ResFac.GetModel(name);
}




//　直列化
void GameObject::Deserialize(json11::Json & jObj)
{
	#pragma region モデル

	if (jObj["ModelFileName"].is_string())
		m_spModel = ResFac.GetModel(jObj["ModelFileName"].string_value());

	#pragma endregion

	#pragma region タグ

	if (jObj["Tag"].is_array())
		m_tag = jObj["Tag"].string_value();

	#pragma endregion

	#pragma region ポジション

	if (jObj["Position"].is_array())
	{
		auto pos = jObj["Position"].array_items();
		m_mWorld.Move(
			(float)pos[0].number_value(),
			(float)pos[1].number_value(),
			(float)pos[2].number_value());
	}
	else
	{
		m_mWorld.CreateIdentity();
	}
	#pragma endregion

	#pragma region ローテーション

	if (jObj["Rotation"].is_array())
	{
		auto rot = jObj["Rotation"].array_items();
		m_mWorld.CreateRotationX((float)D3DXToRadian(rot[0].number_value()));
		m_mWorld.RotateY((float)D3DXToRadian(rot[1].number_value()));
		m_mWorld.RotateZ((float)D3DXToRadian(rot[2].number_value()));
	}

	#pragma endregion

	#pragma region スケーリング

	if (jObj["Scaling"].is_array())
	{
		auto scale = jObj["Scaling"].array_items();
		m_mWorld.ScaleLocal(
			(float)scale[0].number_value(),
			(float)scale[1].number_value(),
			(float)scale[2].number_value());
	}

	#pragma endregion
}

//　インスタンス化
#include"Object\Game\CameraComponent.h"
shared_ptr<GameObject> GameObject::Instantiate(const std::string & className)
{
	if (className == "GameObject")		{ return std::make_shared<GameObject>(); }
	if (className == "CameraComponent") { return std::make_shared<CameraComponent>(); }

	return shared_ptr<GameObject>();
}

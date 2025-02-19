#include"System\Library\define.h"
#include"SceneBase.h"
#include"GameObject.h"

#include"Object\Game\CameraComponent.h"


shared_ptr<GameObject> SceneBase::FindObjectWithTag(const std::string &tag)
{
	for (auto&& obj : m_objects)
	{
		if (obj == nullptr) { continue; }
		if (obj->GetTag() == tag) { return obj; }
	}
	return nullptr;
}

bool SceneBase::IsActive()
{
	if (GetActiveWindow() == NULL)
	{
		return false;
	}
	return true;
}

void SceneBase::Frame()
{
	if (!Update()) { return; }

	D3D.Begin();

	Render3D();

	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�@�X�v���C�g�`��
	SPRITE.Begin();

	Render2D();

	//�@�X�v���C�g�`��I��
	SPRITE.End();
	DEVICE->EndScene();

	DEVICE->Present(nullptr, nullptr, nullptr, nullptr);
}
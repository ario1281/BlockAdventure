#include"System\Library\define.h"
#include"CameraComponent.h"



CameraComponent::CameraComponent()
{
	m_tag = "Camera";
}

void CameraComponent::Deserialize(json11::Json &jObj)
{
	GameObject::Deserialize(jObj);
}

bool CameraComponent::Update(const std::list<shared_ptr<GameObject>>* obj)
{
	if (TASK.GetGoalFlag()) { return true; }
	auto& mCamera = m_mWorld;
	auto owner = m_wpOwner.lock();
	if (owner) { mCamera = owner->GetWorldMatrix(); }

	//�@�����_�̈ʒu�ύX
	mCamera.Move(0, 0.5f, 0);

	//�@�J�����ʒu
	m_camera.GetCamMat() = Matrix();

	//�@��]
	m_camera.GetCamMat().RotateX(D3DXToRadian(CURSOR.Horizontal()));
	m_camera.GetCamMat().RotateY(D3DXToRadian(CURSOR.Vertical()));

	//�@�ړ�
	m_camera.GetCamMat().MoveLocal(m_vOffset);
	m_camera.GetCamMat().Move(mCamera.GetPos());

	m_camera.CameraToView();
	m_camera.CreateParspectiveFovLH(45.0f, 16.0f / 9.0f, 1.0f, 2000.0f);

	D3D.SetCamera(m_camera);

	return true;
}

#pragma once
#include"Game\GameObject.h"

class Switch :public GameObject
{
public:
	Switch();
	~Switch() {}

	void Deserialize(json11::Json& jObj)override;

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override;
	void Draw3D()override;

	void Rotation(const std::list<shared_ptr<GameObject>>* s_obj);


private:
	shared_ptr<DirectSoundManager>	m_sSE	= nullptr;

	std::string						m_dirTag	= "";
	std::string						m_putTag	= "";

	Vector3							m_vAxis = { 1,0,0 };

	float							m_rot		= 0.0f;	//�@�ǂ�ʉ񂷂�(�S��)
	float							m_rot1		= 0.0f;	//�@�ǂ�ʉ񂷂�(1�t���[��)


	//�@�萔
	const float						m_rAng		= 0.5f;	//�@1�t���[���ŉ���
};

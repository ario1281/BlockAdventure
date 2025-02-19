#pragma once
#include"Human.h"

class Hand : public Human
{
public:
	Hand();
	~Hand() {}

	bool Update(const std::list<shared_ptr<GameObject>>* obj)override;
	void Draw3D()override;

	void SetOwner(const shared_ptr<Human>& spOwner) {
		m_wpOwner = spOwner;
	}

private:
	weak_ptr<Human> m_wpOwner;

	const Vector3 m_vOffset = { 0.5f,0.175f,0 };
	Vector3 m_vShake;
	float m_triAng = 0;
	bool m_flg = false;
};
#include"define.h"
#include"LightManager.h"

void LightManager::Init()
{
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	Diffuse(255.0f, 255.0f, 255.0f);
	Specular(255.0f, 255.0f, 255.0f);
	Ambient(255.0f, 255.0f, 255.0f);
	Position(0.0f, 0.0f, 0.0f);
	Direction(0.0f, -1.0, 0.0f);
}

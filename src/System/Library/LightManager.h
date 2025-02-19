#pragma once

class LightManager
{
public:
	LightManager()	{ OutputDebugString("GameObject‚ªŠm•Û"); }
	~LightManager() { OutputDebugString("GameObject‚ª‰ð•ú"); }

	void Init();
	bool Set(int index)
	{
		if (FAILED(DEVICE->SetLight(index, &m_light))) { return false; }
		return{ FAILED(DEVICE->LightEnable(index, TRUE)) ? false : true };
	}

	void Type(D3DLIGHTTYPE type) {
		m_light.Type = type;
	}

	/*======================================
		0~255‚Å‘ã—p
	======================================*/
	void Diffuse(float r, float g, float b, float a = 0) {
		m_light.Diffuse.r = r / 255.0f;
		m_light.Diffuse.g = g / 255.0f;
		m_light.Diffuse.b = b / 255.0f;
		m_light.Diffuse.a = a / 255.0f;
	}
	void Specular(float r, float g, float b, float a = 0) {
		m_light.Specular.r = r / 255.0f;
		m_light.Specular.g = g / 255.0f;
		m_light.Specular.b = b / 255.0f;
		m_light.Specular.a = a / 255.0f;
	}
	void Ambient(float r, float g, float b, float a = 0) {
		m_light.Ambient.r = r / 255.0f;
		m_light.Ambient.g = g / 255.0f;
		m_light.Ambient.b = b / 255.0f;
		m_light.Ambient.a = a / 255.0f;
	}


	void Position(float x, float y, float z = 0)
	{
		m_light.Position = D3DXVECTOR3(x, y, z);
	}
	void Position(Vector3 pos)
	{
		m_light.Position = pos;
	}

	void Direction(float x, float y, float z = 0)
	{
		m_light.Direction = D3DXVECTOR3(x, y, z);
	}
	void Direction(Vector3 vec)
	{
		m_light.Direction = vec;
	}

private:
	D3DLIGHT9 m_light;
};
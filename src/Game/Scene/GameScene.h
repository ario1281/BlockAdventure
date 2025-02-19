#pragma once
#include"Game\Object\Game\Pause.h"

class GameScene : public SceneBase
{
public:
	GameScene() {
		Start();
	}
	~GameScene() {
		SafeClear(m_objects);
	}

	void Start()	override;
	bool Update()	override;
	void Render2D()	override;
	void Render3D()	override;

	void Imgui();

private:
	shared_ptr<Pause>				m_spPause;

	shared_ptr<InputComponent>		m_spInput;
	
	shared_ptr<MeshManager>			m_sky;
	shared_ptr<DirectSoundManager>	m_sBGM;
	Matrix							m_mWorld;

	const float						m_Rotate = 0.0625f;

	bool							m_pauseFlg = false;
	bool							m_clearFg = false;

	//Å@ImGuióp
	bool							m_guiFlg;
};
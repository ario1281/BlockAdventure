#pragma once

class TitleScene : public SceneBase
{
public:
	TitleScene() {
		Start();
	}
	~TitleScene() {}

	void Start()	override;
	bool Update()	override;
	void Render2D()	override;

private:
	shared_ptr<InputComponent>		m_spInput;

	shared_ptr<TextureManager>		m_back;
	shared_ptr<DirectSoundManager>	m_sBGM;
};

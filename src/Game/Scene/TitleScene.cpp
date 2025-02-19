#include"System\Library\define.h"
#include"TitleScene.h"

#include"Game\Object\Title\Logo.h"
#include"Game\Object\Title\Message.h"


void TitleScene::Start()
{
	SafeClear(m_objects);
	m_back = ResFac.GetTexture("data\\title\\title_bg.png");

	//@BGM
	m_sBGM = ResFac.GetSound("data\\bgm\\bgm_1.wav");
	m_sBGM->Play(true);

	m_spInput = std::make_shared<IC_MouseAndKey>();

	//@ƒƒS
	auto spLogo = std::make_shared<TitleLogo>();
	m_objects.push_back(spLogo);
	auto spMsg = std::make_shared<Message>();
	spMsg->SetInput(m_spInput);
	m_objects.push_back(spMsg);
}
bool TitleScene::Update()
{
	if (m_spInput) { m_spInput->Update(); }
	for (auto&& obj : m_objects) { obj->Update(&m_objects); }
	if (APP.GetSceneFlag()) { m_sBGM->Stop(); }

	return true;
}
void TitleScene::Render2D()
{
	Matrix mIden;
	mIden.CreateIdentity();
	SPRITE.DrawTex(mIden, m_back.get());

	for (auto&& obj : m_objects) { obj->Draw2D(); }
}

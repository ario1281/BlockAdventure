#include"System\Library\define.h"
#include"GameScene.h"

#include"Game\Object\Game\CameraComponent.h"
#include"Game\Object\Game\Human.h"
#include"Game\Object\Game\Hand.h"
#include"Game\Object\Game\FieldBase.h"
#include"Game\Object\Game\WoodBlock.h"
#include"Game\Object\Game\StoneBlock.h"
#include"Game\Object\Game\GlassBlock.h"
#include"Game\Object\Game\Switch.h"
#include"Game\Object\Game\GoalStand.h"
#include"Game\Object\Game\Pause.h"



void GameScene::Start()
{
	SafeClear(m_objects);
	m_sky = ResFac.GetModel("data\\field\\back\\bg_sphere.x");

	//　BGM
	m_sBGM = ResFac.GetSound("data\\field\\bgm\\bgm_3.wav");
	m_sBGM->Play(true);

	m_spInput = std::make_shared<IC_MouseAndKey>();

	m_spPause = std::make_shared<Pause>();
	m_spPause.get()->SetInput(m_spInput);
	m_spPause.get()->SetFlag(&m_pauseFlg);

	////　プレイヤー
	//　胴体
	auto spHuman = std::make_shared<Human>();
	spHuman->SetOwner(m_spInput);
	m_objects.push_back(spHuman);
	//　腕部分
	auto spHand = std::make_shared<Hand>();
	spHand->SetOwner(spHuman);
	m_objects.push_back(spHand);

	//　プロック
	auto spField = std::make_shared<FieldBase>();
	m_objects.push_back(spField);

	auto spWood = std::make_shared<Switch>();
	m_objects.push_back(spWood);

	auto spGoal = std::make_shared<GoalStand>();
	m_objects.push_back(spGoal);

	//　カメラ
	auto spCam = std::make_shared<CameraComponent>();
	spCam->SetOwner(spHuman);
	m_objects.push_back(spCam);

	TASK.SetGoalFlag(false);

#ifndef _DEBUG
	int cCnt = 0;
	while (cCnt >= 0) { cCnt = ShowCursor(FALSE); }
#endif // !_DEBUG

}
void GameScene::Imgui()
{

	ImGui::Begin("GameScene_ToolBox",&m_guiFlg, ImGuiWindowFlags_MenuBar);
	//　Menu Bar
	if (ImGui::BeginMenuBar)
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { m_guiFlg = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

bool GameScene::Update()
{
	if (m_spInput) { m_spInput->Update(); }

	//　ゴールしたかどうか
	if(TASK.GetGoalFlag()) { m_sBGM->Stop(); }
	else {
		//　ポーズ処理
		m_spPause.get()->Update(&m_objects);
		if (APP.GetSceneFlag()) { m_sBGM->Stop(); }
		if (m_pauseFlg) { return true; }
		 
		CURSOR.Update();
	}

	for (auto&& obj : m_objects) { obj->Update(&m_objects); }

	//Imgui();
	return true;
}

void GameScene::Render2D()
{
	if (m_pauseFlg) { m_spPause.get()->Draw2D(); }
	for (auto&& obj : m_objects) { obj->Draw2D(); }
}
void GameScene::Render3D()
{
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//　不透明物
	Matrix mIden;
	mIden.Scale(200.0f, 200.0f, 200.0f);
	m_mWorld.RotateY(D3DXToRadian(m_Rotate));
	D3D.SetWorldMatrix(&(mIden * m_mWorld));
	m_sky->Draw();

	for (auto&& obj : m_objects) { obj->Draw3D(); }

	//　透明物
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

}

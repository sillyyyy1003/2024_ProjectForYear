#include "SceneMission.h"

#include "GameApp.h"
#include "KInput.h"
#include "RenderState.h"
#include "ScreenOverlay.h"
#include "UI_Font.h"

void SceneMission::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_mission.json");

	//Mission Paper
	mPaperContainer = std::make_unique<UIStackContainer>();
	mPaperContainer->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mPaperContainer->LoadBackgroundTex(GetObj<Texture>("paper"),{200,50});
	mPaperContainer->LoadFontTexture(GetObj<Texture>("UIFont_Courier_New_It"), UITextOption::fontDefaultSize);
	mPaperContainer->LoadSaveData(sceneData,"Test");

	//Mission Icon
	mMissionOwner = std::make_unique<UIStackContainer>();
	mMissionOwner->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mMissionOwner->LoadBackgroundTex("Assets/Texture/CharIcon/WitchQIcon.png",{128,128});
	mMissionOwner->LoadSaveData(sceneData, "MissionOwner");

	//Accept
	mAccept = std::make_unique<UI_Button>();
	mAccept->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 300,50 }, GetObj<Texture>("UIFont_Courier_New_Bold"), UITextOption::fontDefaultSize);
	mAccept->LoadSaveData(sceneData, "Accept");


	mReject = std::make_unique<UI_Button>();
	mReject->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 300,50 }, GetObj<Texture>("UIFont_Courier_New_Bold"), UITextOption::fontDefaultSize);
	mReject->LoadSaveData(sceneData, "Reject");

}

void SceneMission::UnInit()
{
	json sceneData;

	sceneData["Test"] = mPaperContainer->SaveData("Test");
	sceneData["MissionOwner"] = mMissionOwner->SaveData("MissionOwner");
	sceneData["Accept"] = mAccept->SaveData("Accept");
	sceneData["Reject"] = mReject->SaveData("Reject");
	SaveSceneFile("Assets/Data/SaveDat/scene_mission.json", sceneData);
	
}

void SceneMission::Update(float dt)
{
	mAccept->Update();
	mReject->Update();
	mPaperContainer->Update();
	mMissionOwner->Update();

	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		return;
	}

	if(mAccept->isTrigger()||mReject->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		return;
	}

}

void SceneMission::Draw()
{
	///Back screen over lay
	ScreenOverlay::Get()->ScreenOverlayEffect();

	///Draw Container
	GameApp::SetBlendState(RenderState::BSTransparent);
	mPaperContainer->Draw();
	mMissionOwner->Draw();
	mAccept->Draw();
	mReject->Draw();
	GameApp::SetBlendState(nullptr);

	
}

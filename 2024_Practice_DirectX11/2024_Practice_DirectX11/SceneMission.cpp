#include "SceneMission.h"

#include "GameApp.h"
#include "KInput.h"
#include "MissionManager.h"
#include "RenderState.h"
#include "ScreenOverlay.h"
#include "UI_Button.h"
#include "UI_Font.h"

void SceneMission::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_mission.json");

	//Mission Paper
	mPaperContainer = std::make_unique<UIStackContainer>();
	mPaperContainer->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mPaperContainer->LoadBackgroundTex(GetObj<Texture>("paper"),{200,50});
	mPaperContainer->LoadFontTexture(GetObj<Texture>("UIFont_Courier_New_It"), UITextOption::FONT_DEFAULT_SIZE);
	mPaperContainer->LoadSaveData(sceneData,"Test");

	//Mission Icon
	mMissionOwner = std::make_unique<UIStackContainer>();
	mMissionOwner->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mMissionOwner->LoadBackgroundTex("Assets/Texture/CharIcon/WitchQIcon.png",{128,128});
	mMissionOwner->LoadSaveData(sceneData, "MissionOwner");

	//Accept
	mAccept = std::make_unique<UI_Button>();
	mAccept->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 300,50 }, GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	mAccept->LoadSaveData(sceneData, "Accept");

	//Back Button
	mBack = std::make_unique<UI_Button>();
	mBack->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 300,50 }, GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	mBack->LoadSaveData(sceneData, "Back");


	//Color Sample
	mColorSample = std::make_unique<UIStackContainer>();
	mColorSample->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mColorSample->LoadFile(nullptr, nullptr);
	mColorSample->LoadSaveData(sceneData, "ColorSample");
	Color missionColor = MissionManager::Get()->GetCurrentCheckMission()->GetMissionColor();
	mColorSample->SetBackgroundColor(missionColor);

	mPSWord = std::make_unique<UIStackContainer>();
	mPSWord->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mPSWord->LoadFile(nullptr, GetObj<Texture>("UIFont_Courier_New_It"),{1,1},UITextOption::FONT_DEFAULT_SIZE);
	mPSWord->LoadSaveData(sceneData, "PSWord");


}

void SceneMission::UnInit()
{
	json sceneData;

	sceneData["Test"] = mPaperContainer->SaveData("Test");
	sceneData["MissionOwner"] = mMissionOwner->SaveData("MissionOwner");
	sceneData["Accept"] = mAccept->SaveData("Accept");
	sceneData["Back"] = mBack->SaveData("Back");
	sceneData["ColorSample"] = mColorSample->SaveData("ColorSample");
	sceneData["PSWord"] = mPSWord->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_mission.json", sceneData);
	
}

void SceneMission::Update(float dt)
{
	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		ScreenOverlay::Get()->SetDefaultState();
		return;
	}

	if(mBack->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		ScreenOverlay::Get()->SetDefaultState();
		return;
	}

	if(mAccept->isTrigger())
	{
		MissionManager::Get()->RegisterCheckMissionAsCurrentMission();
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Potion");
		ScreenOverlay::Get()->SetDefaultState();
		return;
	}

	mAccept->Update();
	mBack->Update();
	mPaperContainer->Update();
	mMissionOwner->Update();
	mColorSample->Update();
	mPSWord->Update();
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
	mBack->Draw();
	mColorSample->Draw();
	mPSWord->Draw();


	
}

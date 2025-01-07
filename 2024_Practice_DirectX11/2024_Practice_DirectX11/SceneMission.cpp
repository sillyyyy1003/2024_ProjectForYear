#include "SceneMission.h"

#include "GameApp.h"
#include "KInput.h"
#include "MissionManager.h"
#include "RenderState.h"
#include "ScreenFadeEffect.h"



void SceneMission::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_mission.json");

	mBackground = std::make_unique<UISquare>();
	mBackground->Init(GetObj<Texture>("BlackOverlay"));
	mBackground->LoadDefShader();
	mBackground->LoadSaveData(sceneData["Background"]);

	mPaperBackground = std::make_unique<UISquare>();
	mPaperBackground->Init(GetObj<Texture>("paper3"));
	mPaperBackground->LoadDefShader();
	mPaperBackground->LoadSaveData(sceneData["PaperBackground"]);

	
	//mPaperBackground = std::make_unique<UISquare>();
	mMissionOwner = std::make_unique<UISquare>();

	mColorSample = std::make_unique<D2D_UIStackContainer>();
	MissionName = std::make_unique<D2D_UIStackContainer>();
	MissionOwnerName = std::make_unique<D2D_UIStackContainer>();
	MissionReward = std::make_unique<D2D_UIStackContainer>();

	MissionContent = std::make_unique<D2D_UIStackContainer>();
	MissionOwnerNameContent = std::make_unique<D2D_UIStackContainer>();
	MissionRewardContent = std::make_unique<D2D_UIStackContainer>();


	

	mPaperBackground->Init(GetObj<Texture>("paper3"));
	mPaperBackground->LoadDefShader();
	mPaperBackground->LoadSaveData(sceneData["PaperBackground"]);

	mMissionOwner->Init(GetObj<Texture>("witchIcon1"));
	mMissionOwner->LoadDefShader();
	mMissionOwner->LoadSaveData(sceneData["MissionOwner"]);

	mColorSample->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "ColorSample");
	mColorSample->LoadSaveData(sceneData["ColorSample"]);
	mColorSample->SetUIState(D2DUIConfig::STATE_USE_BACKGROUND);
	D2D1::ColorF color = { MissionManager::Get()->GetCurrentMission().MissionColor.x,MissionManager::Get()->GetCurrentMission().MissionColor.y ,MissionManager::Get()->GetCurrentMission().MissionColor.z ,MissionManager::Get()->GetCurrentMission().MissionColor.w };
	mColorSample->SetBackGroundColor(color);
	
	MissionName->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::SEMI_SIZE, "MissionName");
	MissionName->LoadSaveData(sceneData["MissionName"]);
	MissionName->SetUIState(D2DUIConfig::STATE_USE_FONT);
	MissionName->SetText(MissionManager::Get()->GetCurrentMission().MissionName.c_str());

	MissionOwnerName->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::SEMI_SIZE, "MissionOwnerName");
	MissionOwnerName->LoadSaveData(sceneData["MissionOwnerName"]);
	MissionOwnerName->SetUIState(D2DUIConfig::STATE_USE_FONT);

	MissionReward->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::SEMI_SIZE, "MissionReward");
	MissionReward->LoadSaveData(sceneData["MissionReward"]);
	MissionReward->SetUIState(D2DUIConfig::STATE_USE_FONT);

	MissionContent->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::SMALL_SIZE, "MissionContent");
	MissionContent->LoadSaveData(sceneData["MissionContent"]);
	MissionContent->SetUIState(D2DUIConfig::STATE_USE_FONT);
	MissionContent->SetText(MissionManager::Get()->GetCurrentMission().MissionContent.c_str());

	MissionOwnerNameContent->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::SMALL_SIZE, "MissionNameContent");
	MissionOwnerNameContent->LoadSaveData(sceneData["MissionOwnerNameContent"]);
	MissionOwnerNameContent->SetUIState(D2DUIConfig::STATE_USE_FONT);
	MissionOwnerNameContent->SetText(MissionManager::Get()->GetCurrentMission().MissionOwner.c_str());

	MissionRewardContent->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::SMALL_SIZE, "MissionRewardContent");
	MissionRewardContent->LoadSaveData(sceneData["MissionRewardContent"]);
	MissionRewardContent->SetUIState(D2DUIConfig::STATE_USE_FONT);
	MissionRewardContent->SetText(std::to_string((int)MissionManager::Get()->GetCurrentMission().MissionReward).c_str());

	mWall = std::make_unique<StaticObject>();
	mPaper = std::make_unique<StaticObject>();


	mWall->InitModel("Assets/Texture/brown-cement-concrete_base_1k.jpg", "Wall", PrimitiveConfig::SQUARE);
	mWall->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mWall->SetMaterial({
	{0.3f, 0.3f, 0.3f, 1.0f},
	{0.8f, 0.8f, 0.8f, 1.0f},
	{0.2f, 0.2f, 0.2f, 10.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	TRUE });;

	mPaper->InitModel(GetObj<Texture>("paper3"), "Paper", PrimitiveConfig::SQUARE);
	mPaper->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));






}

void SceneMission::UnInit()
{
//#ifdef _DEBUG
//	json sceneData;
//	sceneData["Background"] = mBackground->SaveData();
//	sceneData["PaperBackground"] = mPaperBackground->SaveData();
//	sceneData["MissionOwner"] = mMissionOwner->SaveData();
//	sceneData["ColorSample"] = mColorSample->SaveData();
//	sceneData["MissionName"] = MissionName->SaveData();
//	sceneData["MissionOwnerName"] = MissionOwnerName->SaveData();
//	sceneData["MissionReward"] = MissionReward->SaveData();
//	sceneData["MissionContent"] = MissionContent->SaveData();
//	sceneData["MissionOwnerNameContent"] = MissionOwnerNameContent->SaveData();
//	sceneData["MissionRewardContent"] = MissionRewardContent->SaveData();
//	SaveSceneFile("Assets/Data/SaveDat/scene_mission.json", sceneData);
//#endif

	json sceneData;
	sceneData["Wall"] = mWall->SaveData();
	sceneData["Paper"] = mPaper->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_mission_test.json", sceneData);
}

void SceneMission::Update(float dt)
{
	accumulateTime += dt;
	mBackground->Update();
	mPaperBackground->Update();

	
	mMissionOwner->Update();

	mColorSample->Update(dt);
	MissionName->Update(dt);
	MissionOwnerName->Update(dt);
	MissionReward->Update(dt);

	MissionContent->Update(dt);
	MissionOwnerNameContent->Update(dt);
	MissionRewardContent->Update(dt);

	pos = sin(accumulateTime * 5.f) * floatingAmplitude;

	mWall->Update(dt);
	mPaper->Update(dt);

	if (!ScreenFadeEffect::Get()->GetFade())
	{
		//シーン切り替えなどトリガーに使われる
		TriggerListener();
	}

	if (ScreenFadeEffect::Get()->GetFadeIn())
	{
		switch (mNextScene)
		{
		case SceneConfig::SceneIndex::SCENE_OPTION:
			SceneManager::Get()->SetMainScene("Option");
			break;
		case SceneConfig::SceneIndex::SCENE_LAB:
			SceneManager::Get()->SetMainScene("Lab");
			break;
		default:
			return;
		}
	}
	
}

void SceneMission::Draw()
{
	//mWall->Draw();
	//mPaper->Draw();
	//MissionManager::Get()->DrawCurrentMissionSet();
	//GameApp::SetBlendState(nullptr);
	//GameApp::SetBlendState(RenderState::BSMulti);
	////mBackground->Draw();
	//GameApp::SetBlendState(nullptr);
	////mPaperBackground->Draw();

	GameApp::SetBlendState(RenderState::BSMulti);
	mBackground->Draw();
	GameApp::SetBlendState(nullptr);
	mPaperBackground->Draw();

	GameApp::SetBlendState(RenderState::BSTransparent);
	mMissionOwner->Draw();
	GameApp::SetBlendState(nullptr);

	mColorSample->Draw();
	MissionName->Draw();
	MissionOwnerName->Draw();
	MissionReward->Draw();
	MissionContent->Draw();
	MissionOwnerNameContent->Draw();
	MissionRewardContent->Draw();

	

}

void SceneMission::TriggerListener()
{
	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetSwitchSceneWithFade(true);
		mNextScene = SceneConfig::SceneIndex::SCENE_LAB;
		return;
	}
}

#include <nlohmann/json.hpp>
#include "SceneTitle.h"
#include <memory>
#include "D2D_UIStackContainer.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "KInput.h"
#include "Model.h"
#include "SceneManager.h"
#include "ScreenFadeEffect.h"


using json = nlohmann::json;
using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneTitle::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_title.json");
	//Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPos({ 0, 5.5f, -14.f });
	GetObj<FirstPersonCamera>("DefaultCamera")->mTransform.SetRotationInRadian(0.35f,0.f,0.f);

#ifdef NDEBUG
	GetObj<FirstPersonCamera>("DefaultCamera")->LockCamera();
#endif

	//Light
	mCandleLight = std::make_unique<CandleLight>();
	mCandleLight->Init();
	mCandleLight->LoadSaveData(sceneData, "CandleLight");


	//Object
	mWater = std::make_unique<Potion>();
	mWater->LoadSaveData(sceneData, "SceneTitleWater");
	mWater->LoadShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mWater->SetTexture(GetObj<Texture>("water"));
	mWater->SetWaterColor({ 1,0,0,0.4f });
	mWater->SetAutoColor(true);
	mWater->SetWaterState(WaterStateConfig::WaterState::STATE_RIPPLING);
	mWater->SetWaterBoilingState(WaterStateConfig::WaterBoilingState::STATE_CONSTANT_BOILING);
	mWater->SetParticleNum(250);
	mWater->InitPotionParticleEffect(0.08f);

	mTitle = std::make_unique<D2D_UIStackContainer>();
	mStart = std::make_unique<UIButton>();
	mOption = std::make_unique<UIButton>();
	mExit = std::make_unique<UIButton>();

	mTitle->Init(D2DUIConfig::UIShape::RECT,D2DUIConfig::FontSize::TITLE_SIZE,"title");
	mTitle->SetUIState(D2DUIConfig::STATE_USE_FONT);
	mTitle->LoadSaveData(sceneData["Title"]);

	mStart->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "start");
	mStart->LoadSaveData(sceneData["Start"]);
	mStart->EnableAllState();

	//mOption->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "Option");
	//mOption->LoadSaveData(sceneData["Option"]);
	//mOption->EnableAllState();

	mExit->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "Exit");
	mExit->LoadSaveData(sceneData["Exit"]);
	mExit->EnableAllState();


	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");
	mPot = std::make_unique<StaticObject>();
	mPot->InitPBR("Assets/Model/LabAssets/Pot.obj", "Pot");
	mPot->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	mPot->LoadTex(pbrTexList);
	mPot->LoadSaveData(sceneData);

}

void SceneTitle::UnInit()
{
#ifdef _DEBUG

	json sceneData;
	sceneData["Title"] = mTitle->SaveData();
	sceneData["Start"] = mStart->SaveData();
	//sceneData["Option"] = mOption->SaveData();
	sceneData["Exit"] = mExit->SaveData();
	sceneData["SceneTitleWater"] = mWater->SaveData();
	sceneData["CandleLight"] = mCandleLight->SaveData();
	sceneData["Pot"] = mPot->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_title.json", sceneData);

#endif
}

void SceneTitle::Update(float dt)
{
	//Object Update / Rendering
	ObjectUpdate(dt);

	if(!ScreenFadeEffect::Get()->GetFade())
	{
		//シーン切り替えなどトリガーに使われる
		TriggerListener();
	}

	if(ScreenFadeEffect::Get()->GetFadeIn())
	{
		switch(mNextScene)
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

void SceneTitle::TriggerListener()
{

	if (mStart->IsTrigger())
	{
		SceneManager::Get()->SetSwitchSceneWithFade(true);
		mNextScene = SceneConfig::SceneIndex::SCENE_LAB;
		return;
	}

	if (mExit->IsTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Exit");
		return;
	}

	if (mOption->IsTrigger())
	{
		SceneManager::Get()->SetSwitchSceneWithFade(true);
		mNextScene = SceneConfig::SceneIndex::SCENE_OPTION;
		return;
	}

}

void SceneTitle::ObjectUpdate(float dt)
{
	
	mTitle->Update(dt);
	mStart->Update(dt);
	//mOption->Update(dt);
	mExit->Update(dt);
	
	mWater->Update(dt);
	mCandleLight->Update(dt);

	mPot->Update(dt);
}

void SceneTitle::Draw()
{
	//SetLightDataToCandle
	Light::PointLight pl[2] = {
		mCandleLight->GetPointLight(),
		{}
	}; 
	GetObj<PixelShader>("PS_PBRModel")->WriteShader(1, pl);
	mPot->Draw();

	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_Primitives")->WriteShader(1, pl);
	mWater->Draw();
	

	mTitle->Draw();
	mStart->Draw();
	//mOption->Draw();
	mExit->Draw();

}


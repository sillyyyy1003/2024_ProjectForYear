#include <nlohmann/json.hpp>
#include <fstream>
#include "SceneTitle.h"
#include <memory>
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "Model.h"
#include "PBRModel.h"
#include "UI_Font.h"
#include "SceneManager.h"


using json = nlohmann::json;
using namespace DirectX::SimpleMath;
using namespace DirectX;



void SceneTitle::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_title.json");
	//Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPos({ 0, 1.2f, -14.f });
	GetObj<FirstPersonCamera>("DefaultCamera")->mTransform.SetRotationInRadian(0.13f,0.f,0.f);

#ifdef NDEBUG
	GetObj<FirstPersonCamera>("DefaultCamera")->LockCamera();
#endif

	//Light
	mCandleLight = std::make_unique<CandleLight>();
	mCandleLight->Init();
	mCandleLight->LoadSaveData(sceneData, "CandleLight");
	//Ui
	uiStart = std::make_unique<UI_Button>();
	uiStart->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 1,1 }, GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	uiStart->LoadSaveData(sceneData, "uiStart");


	uiOption = std::make_unique<UI_Button>();
	uiOption->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 1,1 }, GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	uiOption->LoadSaveData(sceneData, "uiOption");


	uiExit = std::make_unique<UI_Button>();
	uiExit->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 1,1 }, GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	uiExit->LoadSaveData(sceneData, "uiExit");


	uiTitle = std::make_unique<UIStackContainer>();
	uiTitle->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	uiTitle->LoadBackgroundTex(nullptr, { 1,1 });
	uiTitle->LoadFontTexture(GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	uiTitle->LoadSaveData(sceneData, "uiTitle");

	//Object
	mWater = std::make_unique<Potion>();
	mWater->LoadSaveData(sceneData, "ScenePotionWater");
	mWater->LoadShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mWater->SetTexture(GetObj<Texture>("water"));
	mWater->SetWaterColor({1,0,0,0.4f});
	mWater->SetAutoColor(true);
	mWater->SetWaterState(WaterStateConfig::WaterState::STATE_RIPPLING);
	mWater->SetWaterBoilingState(WaterStateConfig::WaterBoilingState::STATE_BOILING);
}

void SceneTitle::UnInit()
{
#ifdef _DEBUG
	json sceneData;
	sceneData["uiStart"] = uiStart->SaveData("uiStart");
	sceneData["uiOption"] = uiOption->SaveData("uiOption");
	sceneData["uiExit"] = uiExit->SaveData("uiExit");
	sceneData["uiTitle"] = uiTitle->SaveData("uiTitle");
	sceneData["ScenePotionWater"] = mWater->SaveData();
	sceneData["CandleLight"] = mCandleLight->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_title.json", sceneData);
#endif

}

void SceneTitle::Update(float dt)
{
	//Object Update / Rendering
	ObjectUpdate(dt);

	//シーン切り替えなどトリガーに使われる
	TriggerListener();
}

void SceneTitle::TriggerListener()
{
	if (uiStart->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
	}

	if (uiExit->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Exit");
	}

	if (uiOption->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Option");
	}
		
}

void SceneTitle::ObjectUpdate(float dt)
{
	uiStart->Update();
	uiOption->Update();
	uiExit->Update();
	uiTitle->Update();

	mWater->Update(dt);
	mCandleLight->Update(dt);
}

void SceneTitle::Draw()
{
	//SetLightDataToCandle
	Light::PointLight pl[2] = {
		mCandleLight->GetPointLight(),
		{}
	};

	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_Primitives")->WriteShader(1, pl);
	mWater->Draw();

	uiStart->Draw();
	uiOption->Draw();
	uiExit->Draw();
	uiTitle->Draw();


}


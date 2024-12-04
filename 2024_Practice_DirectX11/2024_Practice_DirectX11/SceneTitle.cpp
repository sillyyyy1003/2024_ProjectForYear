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
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_title_default.json");

	std::shared_ptr<Texture> uiBgTex = CreateObj<Texture>("uiBg");
	uiBgTex->Create("Assets/Texture/UI/main_title_background_1920x1080.png");

	std::shared_ptr<Texture> uiStartTex = CreateObj<Texture>("uiStart");
	uiStartTex->Create("Assets/Texture/UI/main_title_startbutton_700x140.png");

	std::shared_ptr<Texture> uiOptionTex = CreateObj<Texture>("uiOption");
	uiOptionTex->Create("Assets/Texture/UI/main_title_optionbutton_700x140.png");

	std::shared_ptr<Texture> uiExitTex = CreateObj<Texture>("uiExit");
	uiExitTex->Create("Assets/Texture/UI/main_title_exitbutton_700x140.png");

	uiBg = std::make_shared<UI_Square>();
	uiBg->Init(uiBgTex);
	uiBg->SetScale(WIN_WIDTH, WIN_HEIGHT);
	uiBg->LoadDefShader(GetObj<VertexShader>("VS_DefaultUI"), GetObj<PixelShader>("PS_DefaultUI"));
	uiBg->SetPosZ(2.7f);

	uiStart = std::make_unique<UI_Button>();
	uiStart->Init(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE, uiStartTex, nullptr);
	uiStart->LoadSaveData(sceneData, "uiStart");

	uiOption = std::make_unique<UI_Button>();
	uiOption->Init(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE, uiOptionTex, nullptr);
	uiOption->LoadSaveData(sceneData, "uiOption");

	uiExit = std::make_unique<UI_Button>();
	uiExit->Init(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE,uiExitTex,  nullptr);
	uiExit->LoadSaveData(sceneData, "uiExit");
	
}

void SceneTitle::UnInit()
{
	/*
	json sceneData;
	sceneData["uiStart"]=uiStart->SaveData("uiStart");
	sceneData["uiOption"] = uiOption->SaveData("uiOption");
	sceneData["uiExit"] = uiExit->SaveData("uiExit");
	SaveSceneFile("Assets/Data/SaveDat/scene_title_default.json", sceneData);
	*/
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

	uiBg->Update();
	uiStart->Update();
	uiOption->Update();
	uiExit->Update();

}

void SceneTitle::Draw()
{

	uiBg->Draw();
	uiStart->Draw();
	uiOption->Draw();
	uiExit->Draw();
}


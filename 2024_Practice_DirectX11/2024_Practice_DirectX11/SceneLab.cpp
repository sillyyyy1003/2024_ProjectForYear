#include "SceneLab.h"
#include <memory>
#include "Circle.h"
#include "Container.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "Geometry.h"
#include "GUI.h"
#include "KInput.h"
#include "Object.h"
#include "Square.h"
#include "PointLight.h"
#include "RenderState.h"
#include "UIButton.h"

enum SceneList
{
	SCENE_TITLE,
	SCENE_SHOP,
	SCENE_GUILD,
};

using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneLab::Init()
{
	json sceneData=LoadSceneData("Assets/Data/SaveDat/scene_lab.json");
	//ÉJÉÅÉâçÏê¨
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0, 7, -7);
	GetObj<FirstPersonCamera>("DefaultCamera")->LookAt(GetObj<FirstPersonCamera>("DefaultCamera")->GetPos(), { 0,0,0 }, GetObj<FirstPersonCamera>("DefaultCamera")->GetDefaultUpAxis());
	//camera->LockCameraAngle(true);

	std::shared_ptr<Object> pot = CreateObj<Container>("Pot");
	pot->Init(CYLINDER_ONECAP);
	pot->LoadSaveData(sceneData, "Pot");
	pot->ResetPSShader();


	water = std::make_unique<Water>();
	water->LoadSaveData(sceneData, "Water");


	table = std::make_unique<Square>();
	table->Init("Assets/Texture/Lab/worn_planks_diff_1k.jpg", Vector2(1, 1));
	table->LoadDefShader();
	table->SetScale(Vector2(30, 15));
	table->SetPosition(0.f, -1.0f, 0.0f);

	testIngredient = std::make_unique<Ingredient>();
	testIngredient->Init(SPHERE, "Assets/Texture/Lab/stone_512x512.jpg");
	testIngredient->SetName("test ingredient");
	testIngredient->SetScale(2.0f);


	//===============UI=======================
	infoBox = std::make_unique<UI2D>();
	time = std::make_unique<UI2D>();
	infoBox->InitUI2D(1100, 645, 360, 150);
	infoBox->SetNewFontSize(15.0f);
	time->InitUI2D(260, 120, 500, 200);


	infoButton = std::make_unique<UIButton>();
	infoButton->Init("Assets/Texture/UI/ingredient_usebutton_100x70.png");
	infoButton->InitPosition(Vector3(580.f, 0.0f, 0.1f));
	infoButton->InitCanvasSize(100.f, 70.f);
	
}

void SceneLab::UnInit()
{
	// Create scene Data
	json sceneData;

	//Save Pot
	sceneData["Pot"] = GetObj<Container>("Pot")->SaveData();
	//Save Water
	sceneData["Water"] = water->SaveData();
	//Save Camera
	sceneData["DefaultCamera"] = GetObj<FirstPersonCamera>("DefaultCamera")->SaveData();

	SaveSceneFile("Assets/Data/SaveDat/scene_lab.json", sceneData);
}

void SceneLab::Update(float dt)
{
	GameApp::Get()->GetTimer().GameTick();

	//Input
	if (KInput::IsKeyPress(VK_ESCAPE))
	{
		mSceneIndex = SCENE_TITLE;
		ChangeScene();
		return;
	}

	water->Update(dt);
	table->Update(dt);
	testIngredient->Update(dt);

	GetObj<Container>("Pot")->Update(dt);
	GetObj<FirstPersonCamera>("DefaultCamera")->Update(dt);

	infoBox->Update();
	time->Update();

	infoButton->Update(dt);

	
}

void SceneLab::Draw()
{
	// àÍî 
	std::shared_ptr<Object> obj = GetObj<Container>("Pot");

	GameApp::SetBlendState(nullptr);
	GameApp::SetCullingMode(nullptr);
	table->Draw();
	testIngredient->Draw();

	// ìßñæ
	GameApp::SetCullingMode(RenderState::RSNoCull);		//ó†ï\óºñ ï`âÊÇ∑ÇÈ
	GameApp::SetBlendState(RenderState::BSTransparent);	//ìßñæê›íË
	obj->Draw();
	water->Draw();
	infoButton->Draw();

	std::string info = testIngredient->GetIngredientInfo();
	infoBox->DrawSolidInfoBox(info,D2D1::ColorF::Black,D2D1::ColorF::White);
	std::string gameTime = GameApp::Get()->GetTimer().GetSystemTime();
	time->GetBrush()->SetSolidBrushColor(D2D1::ColorF::Azure);
	time->DrawTextStr(gameTime,SOLID);

}

void SceneLab::ChangeScene()
{
	switch(mSceneIndex)
	{
	default:
	case SCENE_TITLE:
		SceneManager::Get()->SetScene("Title");
		break;
		
	}
}

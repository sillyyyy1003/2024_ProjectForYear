#include "SceneLab.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "KInput.h"
#include "RenderState.h"
#include "ScenePotion.h"
#include "SceneMission.h"


void SceneLab::Init()
{
	//Load Save Data
	json sceneData = LoadSceneData("Assets/Data/SaveDat/SubScene_Lab.json");
	
	//Init Lab Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0, 7, -7);
	GetObj<FirstPersonCamera>("DefaultCamera")->LookAt({ 0,0,7.5 });

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData,"EnvironmentLight");

	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");
	pbrTexList[PBRConfig::PBRTex::AO] = GetObj<Texture>("pbrAO");

	//Init Model & Objects
	mPot = std::make_shared<InteractiveStaticObject>();
	mPot->Init("Assets/Model/LabAssets/Pot.obj", "pot");
	mPot->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_InterActiveObjectPBRModel.cso");
	mPot->LoadTex(pbrTexList);
	mPot->LoadSaveData(sceneData,"Pot");

	std::shared_ptr<StaticObject> Candle1 = CreateObj<StaticObject>("Candle1");
	Candle1->Init("Assets/Model/LabAssets/Candle.obj", "Candle1");
	Candle1->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Candle1->LoadTex(pbrTexList);
	Candle1->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject>	FryingStand = std::make_shared<StaticObject>();
	FryingStand->Init("Assets/Model/LabAssets/FryingStand.obj", "FryingStand");
	FryingStand->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	FryingStand->LoadTex(pbrTexList);
	FryingStand->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Candle2 = CreateObj<StaticObject>("Candle2");
	Candle2->Init("Assets/Model/LabAssets/Candle.obj", "Candle2");
	Candle2->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Candle2->LoadTex(pbrTexList);
	Candle2->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Bottle = CreateObj<StaticObject>("Bottle");
	Bottle->Init("Assets/Model/LabAssets/Bottle.obj", "Bottle");
	Bottle->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Bottle->LoadTex(pbrTexList);
	Bottle->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Jug = CreateObj<StaticObject>("Jug");
	Jug->Init("Assets/Model/LabAssets/Jug.obj", "Jug");
	Jug->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Jug->LoadTex(pbrTexList);
	Jug->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Candle3 = CreateObj<StaticObject>("Candle3");
	Candle3->Init("Assets/Model/LabAssets/Candle.obj", "Candle3");
	Candle3->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Candle3->LoadTex(pbrTexList);
	Candle3->LoadSaveData(sceneData);


	staticObjList[Candle1->GetObjectName()] = Candle1;
	staticObjList[Candle2->GetObjectName()] = Candle2;
	staticObjList[Candle3->GetObjectName()] = Candle3;
	staticObjList[FryingStand->GetObjectName()] = FryingStand;
	staticObjList[Bottle->GetObjectName()] = Bottle;
	staticObjList[Jug->GetObjectName()] = Jug;

	mTable = std::make_unique<Square>();
	mTable->Init("Assets/Texture/Lab/worn_planks_diff_1k.jpg", Vector2(1, 1));
	mTable->LoadDefShader("Assets/Shader/VS_Primitives.cso", "Assets/Shader/PS_Primitives.cso");
	mTable->SetScale(Vector2(30, 15));
	mTable->SetPosition(0.f, 0.0f, 0.0f);

	mWater = std::make_unique<Water>();
	mWater->LoadSaveData(sceneData,"water");
	mWater->SetTexture(GetObj<Texture>("water"));

	testPaper = std::make_unique<MissionPaper>();
	testPaper->Init(GetObj<Texture>("paper"),"paper");

	testPaper->SetPosition({0, 3, 7.5});
	testPaper->SetScale({2,2});
}

void SceneLab::UnInit()
{
	json sceneData;

	for(const auto& obj:staticObjList)
	{
		sceneData[obj.first.c_str()] = obj.second->SaveData();
	}

	sceneData["Pot"] = mPot->SaveData();

	//Save EnvironmentLight
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();
	sceneData["water"] = mWater->SaveData();

	SaveSceneFile("Assets/Data/SaveDat/SubScene_Lab.json",sceneData);
}

void SceneLab::Update(float dt)
{
	TriggerListener();
	GameObjectUpdate(dt);
}

void SceneLab::Draw()
{

	//Use Transparent
	GameApp::SetCullingMode(RenderState::RSNoCull);		//— •\—¼–Ê•`‰æ‚·‚é
	GameApp::SetBlendState(RenderState::BSTransparent);	//“§–¾Ý’è
	mPot->Draw();
	mWater->Draw();

	//Normal
	GameApp::SetCullingMode(nullptr);
	mTable->Draw();
	for (const auto& obj : staticObjList)
	{
		obj.second->Draw();
	}

	testPaper->Draw();
}

void SceneLab::GameObjectUpdate(float dt)
{
	mPot->Update(dt);

	for (const auto& obj : staticObjList)
	{
		obj.second->Update(dt);
	}

	mTable->Update(dt);

	mWater->Update(dt);
	
	testPaper->Update(dt);
}

void SceneLab::TriggerListener()
{

	if(KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetMainScene("Title");
		return;
	}


	if(mPot->GetClicked())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Potion");
	}

	if(testPaper->GetClicked())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Mission");
	}



}


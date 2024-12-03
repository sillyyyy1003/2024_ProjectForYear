#include "ScenePotion.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "InteractiveStaticObject.h"
#include "KInput.h"
#include "SceneManager.h"


void ScenePotion::Init()
{	//Load Save Data
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_potion.json");
	//Init Potion Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0,7.5,0);
	GetObj<FirstPersonCamera>("DefaultCamera")->mTransform.SetRotationInDegree(90,0,0);

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData, "EnvironmentLight");

	mWater = std::make_unique<Water>();
	mWater->LoadSaveData(sceneData, "ScenePotionWater");
	mWater->LoadShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mWater->SetTexture(GetObj<Texture>("water"));

	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");

	//Init Model & Objects
	mPot = std::make_unique<InteractiveStaticObject>();
	mPot->InitPBRModel("Assets/Model/LabAssets/Pot.obj", "pot");
	mPot->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_InterActiveObjectPBRModel.cso");
	mPot->LoadTex(pbrTexList);
}

void ScenePotion::UnInit()
{
	json sceneData;

	//Save EnvironmentLight
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();
	sceneData["ScenePotionWater"] = mWater->SaveData();
	sceneData["Pot"] = mPot->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_potion.json", sceneData);
}

void ScenePotion::Update(float dt)
{
	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		return;
	}

	mWater->Update(dt);
	mPot->Update(dt);
}

void ScenePotion::Draw()
{
	mWater->Draw();
	mPot->Draw();
}

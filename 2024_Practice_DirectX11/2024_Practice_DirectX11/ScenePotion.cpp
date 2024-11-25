#include "ScenePotion.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
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
}

void ScenePotion::UnInit()
{
	json sceneData;

	//Save EnvironmentLight
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();
	sceneData["ScenePotionWater"] = mWater->SaveData();

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
}

void ScenePotion::Draw()
{
	mWater->Draw();
}

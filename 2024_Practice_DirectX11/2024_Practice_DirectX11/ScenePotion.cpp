#include "ScenePotion.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "InteractiveStaticObject.h"
#include "KInput.h"
#include "RenderState.h"
#include "SceneManager.h"


void ScenePotion::Init()
{	//Load Save Data
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_potion.json");
	//Init Potion Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0,10,0);
	GetObj<FirstPersonCamera>("DefaultCamera")->mTransform.SetRotationInDegree(90,0,0);

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData, "EnvironmentLight");


	//Init object
	mWater = CreateObj<Water>("PotionSceneWater");
	mWater->LoadSaveData(sceneData, "ScenePotionWater");
	mWater->LoadShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mWater->SetTexture(GetObj<Texture>("water"));
	mWater->ResetMaterial();

	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");

	//Init Model & Objects
	mPot = std::make_unique<StaticObject>();
	mPot->InitPBR("Assets/Model/LabAssets/Pot.obj", "Pot");
	mPot->LoadSaveData(sceneData);
	mPot->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	mPot->LoadTex(pbrTexList);

	mTable = std::make_unique<StaticObject>();
	mTable->InitModel(GetObj<Texture>("table"), "Table", PrimitiveConfig::SQUARE);
	mTable->LoadShaderFile(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mTable->LoadSaveData(sceneData);

	testIngredientBlue = std::make_unique<Ingredient>();
	testIngredientBlue->Init(nullptr,"Blue",PrimitiveConfig::CIRCLE,{1,1});
	testIngredientBlue->SetColor({ 0,0,1,1 });
	testIngredientBlue->SetPosition({ 3,0.5,0 });

	testIngredientRed = std::make_unique<Ingredient>();
	testIngredientRed->Init(nullptr, "Red", PrimitiveConfig::SQUARE, { 1,1 });
	testIngredientRed->SetColor({ 1,0,0,1 });
	testIngredientRed->SetPosition({ -3,0.5,0 });

	//Set Water
	mWater->SetWaterBoilingState(WaterStateConfig::WaterBoilingState::STATE_BOILING);
	mWater->SetWaterState(WaterStateConfig::WaterState::STATE_RIPPLING);
}

void ScenePotion::UnInit()
{
	json sceneData;

	//Save EnvironmentLight
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();
	sceneData["ScenePotionWater"] = mWater->SaveData();
	sceneData["Pot"] = mPot->SaveData();
	sceneData["Table"] = mTable->SaveData();
	//sceneData["Red"] = testIngredientRed->SaveData();
	sceneData["Blue"] = testIngredientBlue->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_potion.json", sceneData);
}

void ScenePotion::Update(float dt)
{
	TriggerListener();

	GameObjectUpdate(dt);
}

void ScenePotion::Draw()
{
	mTable->Draw();
	testIngredientRed->Draw();
	testIngredientBlue->Draw();

	GameApp::SetBlendState(RenderState::BSTransparent);
	mPot->Draw();
	mWater->Draw();

}

void ScenePotion::GameObjectUpdate(float dt)
{
	mWater->Update(dt);
	mPot->Update(dt);
	mTable->Update(dt);

	testIngredientBlue->Update(dt);
	testIngredientRed->Update(dt);
}

void ScenePotion::TriggerListener()
{
	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		return;
	}
}

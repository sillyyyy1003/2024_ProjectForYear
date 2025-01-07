﻿#include "SceneManager.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "Geometry.h"
#include "IngredientManager.h"
#include "MissionManager.h"
#include "PBRModel.h"
#include "SceneLab.h"
#include "SceneMission.h"
#include "SceneOption.h"
#include "ScenePotion.h"
#include "SceneTitle.h"
#include "ScreenFadeEffect.h"
#include "TutorialManager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneManager::SetCurrentCamera(std::shared_ptr<CameraBase> camera)
{
	mCurrentCamera = camera;
}

void SceneManager::Init()
{
	//LoadSaveData
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_manager.json");
	json playerData = LoadSceneData("Assets/Data/SaveDat/player_data.json");
	json tutorialData = LoadSceneData("Assets/Data/SaveDat/tutorial_data.json");
	json missionData = LoadSceneData("Assets/Data/SaveDat/mission_data.json");

	//Set Scene Map
	InitSceneMap();

	//カメラ作成
	std::shared_ptr<FirstPersonCamera> camera = CreateObj<FirstPersonCamera>("DefaultCamera");
	camera->SetPosition(0.0, 10.0, -10.0);
	camera->LookAt(camera->GetPos(), { 0,0,0 }, camera->GetDefaultUpAxis());
	//現在のカメラをセットする
	SetCurrentCamera(camera);

	//基底ライト作成 Ambient Light
	std::shared_ptr <DirLight> light = CreateObj<DirLight>("EnvironmentLight");
	light->SetAmbient(Vector4(0.5f,0.5f,0.5f,1.0f));
	light->SetPosition(Vector3(0.0f,10.0f,-10.0f));
	light->SetDirection(Vector3(0.0f,0.0,0.f));

	//Font Lib 作成
	InitFontLib();
	InitModelTexture();

	//Load PS/VS file
	LoadPixelShaderFile();
	LoadVertexShaderFile();

	//Init Fade Tool
	ScreenFadeEffect::Get()->Init();
	ScreenFadeEffect::Get()->SetState(ScreenOverlayConfig::STATE_FADE_IN);

	//Init Player
	mPlayer = CreateObj<Player>("player");
	mPlayer->LoadPlayerData(playerData["Player"]);

	//Init Tutorial
	TutorialManager::Get()->Init(tutorialData["Tutorial"]);

	//Init Ingredient Manager
	IngredientManager::Get()->Init();

	//Load MissionFalse
	MissionManager::Get()->Init();
	MissionManager::Get()->LoadMissionList(missionData);

	//Set Scene
	SetMainScene("Title");

	

}

void SceneManager::UnInit()
{
	json sceneData;
	sceneData["DefaultCamera"] = GetObj<FirstPersonCamera>("DefaultCamera")->SaveData();
	//Save data here
	SaveSceneFile("Assets/Data/SaveDat/scene_manager.json", sceneData);

	json playerData;
	playerData["Player"] = mPlayer->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/player_data.json", playerData);

	json tutorialData;
	tutorialData["Tutorial"]=TutorialManager::Get()->UnInit();
	SaveSceneFile("Assets/Data/SaveDat/tutorial_data.json", tutorialData);

	json missionData;
	missionData = MissionManager::Get()->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/mission_data.json", missionData);
}

void SceneManager::Update(float dt)
{
	GetObj<FirstPersonCamera>("DefaultCamera")->Update(dt);
	GetObj<DirLight>("EnvironmentLight")->Update(dt);

	ScreenFadeEffect::Get()->Update(dt);
	IngredientManager::Get()->Update(dt);
	MainSceneChangeListener();
}

void SceneManager::Draw()
{
	ScreenFadeEffect::Get()->Draw();
}

void SceneManager::SetSwitchScene(bool isSwitchScene)
{
	this->isSwitchScene = isSwitchScene;
}

void SceneManager::SetSwitchSceneWithFade(bool isSwitchScene)
{
	this->isSwitchScene = isSwitchScene;
	ScreenFadeEffect::Get()->SetState(ScreenOverlayConfig::STATE_FADE_OUT);
}

void SceneManager::SetMainScene(const std::string& sceneName)
{
	if (mSceneMap.contains(sceneName))
	{
		mSceneIndex = mSceneMap[sceneName];
		ChangeMainScene();
	}
}

void SceneManager::InitSceneMap()
{	//Set SceneMap
	mSceneMap["Title"] = SceneConfig::SceneIndex::SCENE_TITLE;
	mSceneMap["Lab"] = SceneConfig::SceneIndex::SCENE_LAB;
	mSceneMap["Option"] = SceneConfig::SceneIndex::SCENE_OPTION;
	mSceneMap["Exit"] = SceneConfig::SceneIndex::SCENE_EXIT;
	mSceneMap["Potion"] = SceneConfig::SceneIndex::SCENE_POTION;
	mSceneMap["Mission"] = SceneConfig::SceneIndex::SCENE_MISSION;
	mSceneMap["InGameOption"] = SceneConfig::SceneIndex::SCENE_IN_GAME_OPTION;

}

void SceneManager::InitFontLib()
{
	//UI_Font
	UIFont_OCRA_Extend = CreateObj<Texture>("UIFont_OCRA_Extend");
	HR(UIFont_OCRA_Extend->Create("Assets/Texture/UI/UIFontLib/ASCIILib_OCRA_Extend.png"));
}

void SceneManager::InitModelTexture()
{
	fantasyHousePropsAlbedoTexture = CreateObj<Texture>("pbrAlbedo");
	HR(fantasyHousePropsAlbedoTexture->Create("Assets/Model/LabAssets/TrimSheets_mTrimSheet_AlbedoTransparency.png"));

	fantasyHousePropsMetallicTexture = CreateObj<Texture>("pbrMetallic");
	HR(fantasyHousePropsMetallicTexture->Create("Assets/Model/LabAssets/TrimSheets_mTrimSheet_MetallicSmoothness.png"));

	fantasyHousePropsNormalTexture = CreateObj<Texture>("pbrNormal");
	HR(fantasyHousePropsNormalTexture->Create("Assets/Model/LabAssets/TrimSheets_mTrimSheet_Normal.png"));

	waterTexture = CreateObj<Texture>("water");
	HR(waterTexture->Create("Assets/Texture/water.png"));

	paperTexture = CreateObj<Texture>("paper");
	HR(paperTexture->Create("Assets/Texture/sepia-plasterboard-texture1.jpg"));

	Texture* paperTexture2 = CreateObj<Texture>("paper2").get();
	HR(paperTexture2->Create("Assets/Texture/paper2.png"));

	Texture* paperTexture3 = CreateObj<Texture>("paper3").get();
	HR(paperTexture3->Create("Assets/Texture/paper3.png"));

	Texture* paperTexture4=CreateObj<Texture>("paper4").get();
	HR(paperTexture4->Create("Assets/Texture/paper4.png"));

	Texture* splashTexture = CreateObj<Texture>("splash").get();
	HR(splashTexture->Create("Assets/Texture/Splash.png"));

	Texture* witchIcon = CreateObj<Texture>("witchIcon1").get();
	HR(witchIcon->Create("Assets/Texture/CharIcon/WitchChar.png"));

	blackOverlay=CreateObj<Texture>("BlackOverlay");
	HR(blackOverlay->Create("Assets/Texture/ScreenOverlay/bg_mask.png"));

	Texture* fadeOverlay = CreateObj<Texture>("Fade").get();
	HR(fadeOverlay->Create("Assets/Texture/ScreenOverlay/Fade.png"));


	//Close Book
	/*closeBookRedAlbedoTexture = CreateObj<Texture>("closeBookRedAlbedo");
	HR(closeBookRedAlbedoTexture->Create("Assets/Model/Book/book_close_book_close_BaseColor.1001_00.png"));

	closeBookBlueAlbedoTexture = CreateObj<Texture>("closeBookBlueAlbedo");
	HR(closeBookBlueAlbedoTexture->Create("Assets/Model/Book/book_close_book_close_BaseColor.1001.png"));

	closeBookNormalTexture = CreateObj<Texture>("closeBookNormal");
	HR(closeBookNormalTexture->Create("Assets/Model/Book/book_close_book_close_Normal.1001.png"));

	closeBookMetallicTexture = CreateObj<Texture>("closeBookMetallic");
	HR(closeBookMetallicTexture->Create("Assets/Model/Book/book_close_book_close_Roughness.1001.png"));*/

	//Open Book
	//openBookRedAlbedoTexture = CreateObj<Texture>("openBookRedAlbedo");
	//HR(openBookRedAlbedoTexture->Create("Assets/Model/Book/book_open_book_open_BaseColor.1001_00.png"));

	//openBookBlueAlbedoTexture = CreateObj<Texture>("openBookBlueAlbedo");
	//HR(openBookBlueAlbedoTexture->Create("Assets/Model/Book/book_open_book_open_BaseColor.1001_01.png"));

	//openBookNormalTexture = CreateObj<Texture>("openBookNormal");
	//HR(openBookBlueAlbedoTexture->Create("Assets/Model/Book/book_open_book_open_Normal.1001.png"));

	//openBookMetallicTexture = CreateObj<Texture>("openBookMetallic");
	//HR(openBookMetallicTexture->Create("Assets/Model/Book/book_open_book_open_Roughness.1001.png"));

	tableTexture = CreateObj<Texture>("table");
	HR(tableTexture->Create("Assets/Texture/Lab/worn_planks_diff_1k.jpg"));

}


void SceneManager::LoadPixelShaderFile()
{
	PS_buttonUI = CreateObj<PixelShader>("PS_ButtonUI");
	PS_buttonUI->LoadShaderFile("Assets/Shader/PS_ButtonUI.cso");

	PS_defaultUI = CreateObj<PixelShader>("PS_DefaultUI");
	PS_defaultUI->LoadShaderFile("Assets/Shader/PS_DefaultUI.cso");

	PS_primitives = CreateObj<PixelShader>("PS_Primitives");
	PS_primitives->LoadShaderFile("Assets/Shader/PS_Primitives.cso");

	PS_pbrModel = CreateObj<PixelShader>("PS_PBRModel");
	PS_pbrModel->LoadShaderFile("Assets/Shader/PS_PBRModel.cso");

	PS_interactiveStaticPBR = CreateObj<PixelShader>("PS_InterActiveObjectPBRModel");
	PS_interactiveStaticPBR->LoadShaderFile("Assets/Shader/PS_InterActiveObjectPBRModel.cso");

	PS_interactiveStaticNormal = CreateObj<PixelShader>("PS_InteractiveObjectNormal");
	PS_interactiveStaticNormal->LoadShaderFile("Assets/Shader/PS_InteractiveObjectNormal.cso");

	PS_Shadow = CreateObj<PixelShader>("PS_Shadow");
	PS_Shadow->LoadShaderFile("Assets/Shader/PS_Shadow.cso");

	PS_WriteDepth = CreateObj<PixelShader>("PS_WriteDepth");
	PS_WriteDepth->LoadShaderFile("Assets/Shader/PS_WriteDepth.cso");

	PS_pbrBaseColor = CreateObj<PixelShader>("PS_OpenGLBaseColorModel");
	PS_pbrBaseColor->LoadShaderFile("Assets/Shader/PS_OpenGLBaseColorModel.cso");

	PixelShader* PS_water = CreateObj<PixelShader>("PS_Water").get();
	PS_water->LoadShaderFile("Assets/Shader/PS_Water.cso");

	PixelShader* PS_Outline = CreateObj<PixelShader>("PS_Outline").get();
	PS_Outline->LoadShaderFile("Assets/Shader/PS_Outline.cso");

	PixelShader* PS_Ingredient = CreateObj<PixelShader>("PS_Ingredient").get();
	PS_Ingredient->LoadShaderFile("Assets/Shader/PS_Ingredient.cso");


}

void SceneManager::LoadVertexShaderFile()
{
	VS_defaultUI = CreateObj<VertexShader>("VS_DefaultUI");
	VS_defaultUI->LoadShaderFile("Assets/Shader/VS_DefaultUI.cso");

	VS_pbrModel = CreateObj<VertexShader>("VS_PBRModel");
	VS_pbrModel->LoadShaderFile("Assets/Shader/VS_PBRModel.cso");

	VS_primitives = CreateObj<VertexShader>("VS_Primitives");
	VS_primitives->LoadShaderFile("Assets/Shader/VS_Primitives.cso");

	VS_spriteShadow = CreateObj<VertexShader>("VS_SpriteShadow");
	VS_spriteShadow->LoadShaderFile("Assets/Shader/VS_SpriteShadow.cso");

}

void SceneManager::ChangeMainScene()
{
	isChangeMainScene = true;
}

void SceneManager::ClearMainSceneChange()
{
	isChangeMainScene = false;

}

void SceneManager::MainSceneChangeListener()
{

	if (isChangeMainScene)
	{
		switch (mSceneIndex)
		{
		
		case SceneConfig::SceneIndex::SCENE_TITLE:AddSubScene<SceneTitle>();
			DebugLog::Log("SceneName = SceneTitle");
			break;
		case SceneConfig::SceneIndex::SCENE_LAB:AddSubScene<SceneLab>();
			DebugLog::Log("SceneName = SceneLab");
			break;
		case SceneConfig::SceneIndex::SCENE_OPTION:AddSubScene<SceneOption>();
			DebugLog::Log("SceneName = SceneOption");
			break;
		case SceneConfig::SceneIndex::SCENE_MISSION:AddSubScene<SceneMission>();
			DebugLog::Log("SceneName = SceneMission");
			break;
		case SceneConfig::SceneIndex::SCENE_POTION:AddSubScene<ScenePotion>();
			DebugLog::Log("SceneName = ScenePotion");
			break;
		case SceneConfig::SceneIndex::SCENE_EXIT:
			DebugLog::Log("Exiting the application...");
			PostQuitMessage(0);
			break;
		default:
			return;
		}

		ClearMainSceneChange();
	}
}

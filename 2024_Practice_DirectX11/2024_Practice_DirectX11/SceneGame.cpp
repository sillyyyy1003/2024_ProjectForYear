#include "SceneGame.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "Geometry.h"
#include "GUI.h"
#include "KInput.h"
#include "Object.h"
#include "Square.h"
#include "PointLight.h"
#include "RenderState.h"
#include "SceneLab.h"
#include "UIButton.h"
#include <ScenePotion.h>
#include "SceneMission.h"


using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneGame::Init()
{
	json sceneData=LoadSceneData("Assets/Data/SaveDat/scene_lab.json");
	//ÉJÉÅÉâçÏê¨
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0, 7, -7);
	GetObj<FirstPersonCamera>("DefaultCamera")->LookAt(GetObj<FirstPersonCamera>(
		"DefaultCamera")->GetPos(),
		{ 0,0,0 }, 
		GetObj<FirstPersonCamera>("DefaultCamera")->GetDefaultUpAxis());


	//===============UI=======================

	//Scene ê›íË
	//SceneManager::Get()->S("Lab");
	//ChangeScene();
	
}

void SceneGame::UnInit()
{
	// Create scene Data
	json sceneData;

	//Save Water
	//sceneData["Water"] = water->SaveData();
	//Save Camera
	sceneData["DefaultCamera"] = GetObj<FirstPersonCamera>("DefaultCamera")->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_lab.json", sceneData);
}

void SceneGame::Update(float dt)
{

	GameApp::Get()->GetTimer().GameTick();

	TriggerListener();

	GameObjectUpdate(dt);


}

void SceneGame::Draw()
{
	
}

void SceneGame::TriggerListener()
{
	////SceneÇÃêÿÇËë÷Ç¶
	//if (SceneManager::Get()->GetGameSubSceneSwitch())
	//{
	//	ChangeScene();
	//	SceneManager::Get()->SetGameSubSceneSwitch(false);
	//}

}

void SceneGame::GameObjectUpdate(float dt)
{

}

void SceneGame::ChangeScene()
{
	//switch (SceneManager::Get()->GetCurrentSubScene())
	//{
	//	default:
	//	case SceneConfig::GameSubScene::SCENE_LAB:AddSubScene<SceneLab>();
	//		DebugLog::Log("SceneName = SceneLab");
	//		break;
	//
	//	case SceneConfig::GameSubScene::SCENE_POTION:AddSubScene<ScenePotion>();
	//		DebugLog::Log("SceneName = ScenePotion");
	//		break;
	//	case SceneConfig::GameSubScene::SCENE_MISSION:AddSubScene<SceneMission>();
	//		DebugLog::Log("SceneName = SceneMission");
	//		break;
	//}
}

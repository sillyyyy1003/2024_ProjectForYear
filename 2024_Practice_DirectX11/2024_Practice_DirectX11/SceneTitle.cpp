#include <nlohmann/json.hpp>
#include <fstream>
#include "SceneTitle.h"
#include "FirstPersonCamera.h"
#include "CanvasUI.h"
#include "SceneLab.h"
#include "SceneManager.h"
using json = nlohmann::json;

using namespace DirectX::SimpleMath;
using namespace DirectX;

enum SceneList
{
	SCENE_NONE = 0 ,
	SCENE_MAIN,
	SCENE_OPTION,
	EXIT,
};

void SceneTitle::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_title_default.json");

	CanvasUI* uiBg = CreateObj<CanvasUI>("title_bg");
	uiBg->LoadSaveData(sceneData, "title_bg");

	CanvasUI* uiStart = CreateObj<UI_Button>("title_start");
	uiStart->LoadSaveData(sceneData, "title_start");

	CanvasUI* uiOption = CreateObj<UI_Button>("title_option");
	uiOption->LoadSaveData(sceneData, "title_option");

	CanvasUI* uiExit = CreateObj<UI_Button>("title_exit");
	uiExit->LoadSaveData(sceneData, "title_exit");

	//Set data to map
	uiManager["background"].push_back(uiBg);  
	uiManager["button"].push_back(uiStart);   
	uiManager["button"].push_back(uiOption);  
	uiManager["button"].push_back(uiExit);

	//描画順番の設定
	mUiOrder = { "background", "button" ,"effect" };

	mSceneIndex = SCENE_NONE;
}

void SceneTitle::UnInit()
{
	//
	if(isEditable)
	{
		// Create scene Data
		json sceneData;

		for (auto it = uiManager.begin(); it != uiManager.end(); ++it)
		{
			for (auto& element : it->second)
			{
				sceneData[element->GetObjectName().c_str()] = element->SaveData(element->GetFilePath().c_str());
			}

		}

		SaveSceneFile("Assets/Data/SaveDat/scene_title_default.json", sceneData);
	}

	//
}

void SceneTitle::Update(float dt)
{
#ifdef _DEBUG

	if(ImGui::Begin("EditCheck"))
	{
		ImGui::Checkbox("isEditable", &isEditable);
	}

	ImGui::End();
#endif

	//ボタンの状態変更
	for (const std::string& category : mUiOrder)
	{
		if (uiManager.contains(category))
		{
			for (CanvasUI* obj : uiManager[category])
			{
				obj->Update(dt);
			}
		}
	}

	//シーンの切り替え//todo can be automatic
	UI_Button* startButton = GetObj<UI_Button>("title_start");
	if(startButton->isTrigger())
		mSceneIndex = SCENE_MAIN;

	UI_Button* exitButton = GetObj<UI_Button>("title_exit");
	if(exitButton->isTrigger())
		mSceneIndex = EXIT;

	if (mSceneIndex != SCENE_NONE)
		ChangeScene();

}

void SceneTitle::Draw()
{
	float currentZ = UI_FARZ - 0.1f;
	for (const std::string& category : mUiOrder)
	{
		// 获取该类别下的所有 UI 对象
		if (uiManager.contains(category))
		{
			for (CanvasUI* obj : uiManager[category])
			{
				obj->SetPosZ(currentZ);
				obj->Draw();          
				currentZ -= 0.1f;      
			}
		}
	}
}

void SceneTitle::ChangeScene()
{
	switch (mSceneIndex)
	{
	default:
	case SCENE_MAIN:
		//ENTER A NEW SCENE
		SceneManager::Get()->SetScene("Lab");
		break;
	case SCENE_OPTION:
		
	case EXIT:
		DebugLog::Log("Exiting the application...");
		PostQuitMessage(0); 
		break;
	}
}

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

enum TitleScene
{
	SCENE_NONE = 0 ,
	SCENE_MAIN,
	SCENE_OPTION,
	EXIT,
};

void SceneTitle::Init()
{
	std::filesystem::path filePath = "Assets/Data/SaveDat/scene_title_default.json";
	if (!std::filesystem::exists(filePath))
	{
		DebugLog::Log("scene_title.json not found.");
		return;
	}

	std::ifstream file(filePath);
	if (!file.is_open())
	{
		DebugLog::Log("Failed to open scene_title.json for reading.");
		return;
	}

	json sceneData;
	file >> sceneData;
	file.close();

	//Setting
	//CanvasUI* uiBg = LoadData2D<CanvasUI>("title_bg", sceneData);
	//CanvasUI* uiStart = LoadData2D<UI_Button>("title_start", sceneData);
	//CanvasUI* uiOption = LoadData2D<UI_Button>("title_option", sceneData);
	//CanvasUI* uiExit = LoadData2D<UI_Button>("title_exit", sceneData);

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
	// Create scene Data
	json sceneData;

	/*sceneData["title_bg"] = SaveData(GetObj<CanvasUI>("title_bg"));
	sceneData["title_start"] = SaveData(GetObj<UI_Button>("title_start"));
	sceneData["title_option"] = SaveData(GetObj<UI_Button>("title_option"));
	sceneData["title_exit"] = SaveData(GetObj<UI_Button>("title_exit"));*/

	for(auto it =uiManager.begin();it!=uiManager.end();++it)
	{
		for(auto& element:it->second)
		{
			sceneData[element->GetObjectName().c_str()] = element->SaveData(element->GetFilePath().c_str());
		}
		
	}


	std::ofstream file("Assets/Data/SaveDat/scene_title_default.json");
	if (file.is_open())
	{
		file << sceneData.dump(4);
		file.close();
	}
	else
	{
		DebugLog::Log("Failed to open scene_title.json for writing.");
	}


}

void SceneTitle::Update(float dt)
{
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
	if(startButton->isPressed())
		mSceneIndex = SCENE_MAIN;

	UI_Button* exitButton = GetObj<UI_Button>("title_exit");
	if(exitButton->isPressed())
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

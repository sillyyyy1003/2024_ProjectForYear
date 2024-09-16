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
	std::filesystem::path filePath = "Assets/Data/SaveDat/scene_title.json";
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
	CanvasUI* uiBg = CreateObj<CanvasUI>("title_bg");
	uiBg->InitResource(sceneData["title_bg"]["filepath"].get<std::string>().c_str());

	CanvasUI* uiStart = CreateObj<UI_Button>("title_start");
	uiStart->InitResource(sceneData["title_start"]["filepath"].get<std::string>().c_str());
	Vector3 pos = Vector3(sceneData["title_start"]["position"][0],sceneData["title_start"]["position"][1] ,sceneData["title_start"]["position"][2]);
	uiStart->InitData(pos);

	CanvasUI* uiOption = CreateObj<UI_Button>("title_option");
	uiOption->InitResource(sceneData["title_option"]["filepath"].get<std::string>().c_str());
	pos = Vector3( sceneData["title_option"]["position"][0],sceneData["title_option"]["position"][1] ,sceneData["title_option"]["position"][2]);
	uiOption->InitData(pos);

	 
	CanvasUI* uiExit = CreateObj<UI_Button>("title_exit");
	uiExit->InitResource(sceneData["title_exit"]["filepath"].get<std::string>().c_str());
	pos = Vector3(sceneData["title_exit"]["position"][0], sceneData["title_exit"]["position"][1], sceneData["title_exit"]["position"][2]);
	uiExit->InitData(pos);

	//Set data to map
	uiManager["background"].push_back(uiBg);  
	uiManager["button"].push_back(uiStart);   
	uiManager["button"].push_back(uiOption);  
	uiManager["button"].push_back(uiExit);

	//描画順番の設定
	mUiOrder = { "background",  "button" ,"effect" };

	mSceneIndex = SCENE_NONE;
}

void SceneTitle::UnInit()
{
	// Create scene Data
	json sceneData;

	// Save info
	json uiData[4];
	uiData[0]["name"] = "title_bg";
	uiData[0]["position"] = { GetObj<CanvasUI>("title_bg")->GetPos().x, GetObj<CanvasUI>("title_bg")->GetPos().y, GetObj<CanvasUI>("title_bg")->GetPos().z };
	uiData[0]["filepath"] = GetObj<CanvasUI>("title_bg")->GetFilePath();
	sceneData["title_bg"] = uiData[0];


	// title_start
	uiData[1]["name"] = "title_start";
	uiData[1]["position"] = { GetObj<UI_Button>("title_start")->GetPos().x, GetObj<UI_Button>("title_start")->GetPos().y, GetObj<UI_Button>("title_start")->GetPos().z };
	uiData[1]["filepath"] = GetObj<UI_Button>("title_start")->GetFilePath();
	sceneData["title_start"] = uiData[1];

	// title_option
	uiData[2]["name"] = "title_option";
	uiData[2]["position"] = { GetObj<UI_Button>("title_option")->GetPos().x, GetObj<UI_Button>("title_option")->GetPos().y, GetObj<UI_Button>("title_option")->GetPos().z };
	uiData[2]["filepath"] = GetObj<UI_Button>("title_option")->GetFilePath();
	sceneData["title_option"] = uiData[2];

	// title_exit
	uiData[3]["name"] = "title_exit";
	uiData[3]["position"] = { GetObj<UI_Button>("title_exit")->GetPos().x, GetObj<UI_Button>("title_exit")->GetPos().y, GetObj<UI_Button>("title_exit")->GetPos().z };
	uiData[3]["filepath"] = GetObj<UI_Button>("title_exit")->GetFilePath();
	sceneData["title_exit"] = uiData[3];


	std::ofstream file("Assets/Data/SaveDat/scene_title.json");
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

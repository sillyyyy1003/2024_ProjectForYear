#include <nlohmann/json.hpp>
#include <fstream>
#include "SceneTitle.h"

#include <memory>

#include "FirstPersonCamera.h"
#include "Model.h"
#include "PBRModel.h"
#include "UIFont.h"
#include "SceneGame.h"
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

	uiBg = std::make_shared<CanvasUI>();
	uiBg->LoadSaveData(sceneData, "title_bg");

	uiStart = std::make_shared<UIButton>();
	uiStart->LoadSaveData(sceneData, "title_start");

	uiOption = std::make_shared<UIButton>();
	uiOption->LoadSaveData(sceneData, "title_option");

	uiExit = std::make_shared<UIButton>();
	uiExit->LoadSaveData(sceneData, "title_exit");

	//Set data to map
	objManager["background"].push_back(uiBg);  
	objManager["button"].push_back(uiStart);   
	objManager["button"].push_back(uiOption);  
	objManager["button"].push_back(uiExit);

	//描画順番の設定
	mUiOrder = { "background", "button" ,"effect" };

	
}

void SceneTitle::UnInit()
{
	if(isEditable)
	{
		// Create scene Data
		json sceneData;

		for (auto it = objManager.begin(); it != objManager.end(); ++it)
		{
			for (auto& element : it->second)
			{
				sceneData[element->GetObjectName().c_str()] = element->SaveData(element->GetFilePath().c_str());
			}

		}
		SaveSceneFile("Assets/Data/SaveDat/scene_title_default.json", sceneData);
	}
}

void SceneTitle::Update(float dt)
{
	//Object Update / Rendering
	ObjectUpdate(dt);

	//シーン切り替えなどトリガーに使われる
	TriggerListener();
}

void SceneTitle::TriggerListener()
{
	if (uiStart->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
	}

	if (uiExit->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Exit");
	}

	if (uiOption->isTrigger())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Option");
	}
		
}

void SceneTitle::ObjectUpdate(float dt)
{

	


#ifdef _DEBUG

	if (ImGui::Begin("EditCheck"))
	{
		ImGui::Checkbox("isEditable", &isEditable);
	}

	ImGui::End();
#endif

	//ボタンの状態変更
	for (const std::string& category : mUiOrder)
	{
		if (objManager.contains(category))
		{
			for (auto obj : objManager[category])
			{
				obj->Update(dt);
			}
		}
	}




}

void SceneTitle::Draw()
{
	float currentZ = UI_FARZ - 0.1f;

	for (const std::string& category : mUiOrder)
	{
		// 获取该类别下的所有 UI 对象
		if (objManager.contains(category))
		{
			for (auto& obj : objManager[category])
			{
				obj->SetPosZ(currentZ);
				obj->Draw();          
				currentZ -= 0.1f;      
			}
		}
	}
	
}


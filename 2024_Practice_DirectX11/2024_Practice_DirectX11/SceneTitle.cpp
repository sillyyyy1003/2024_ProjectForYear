#include <nlohmann/json.hpp>
#include <fstream>
#include "SceneTitle.h"
#include "FirstPersonCamera.h"
#include "Model.h"
#include "PBRModel.h"
#include "UIFont.h"
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

	std::shared_ptr<CanvasUI> uiBg = CreateObj<CanvasUI>("title_bg");
	uiBg->LoadSaveData(sceneData, "title_bg");

	std::shared_ptr<CanvasUI> uiStart = CreateObj<UIButton>("title_start");
	uiStart->LoadSaveData(sceneData, "title_start");

	std::shared_ptr<CanvasUI> uiOption = CreateObj<UIButton>("title_option");
	uiOption->LoadSaveData(sceneData, "title_option");

	std::shared_ptr<CanvasUI> uiExit = CreateObj<UIButton>("title_exit");
	uiExit->LoadSaveData(sceneData, "title_exit");

	//Set data to map
	uiManager["background"].push_back(uiBg);  
	uiManager["button"].push_back(uiStart);   
	uiManager["button"].push_back(uiOption);  
	uiManager["button"].push_back(uiExit);

	//描画順番の設定
	mUiOrder = { "background", "button" ,"effect" };

	mSceneIndex = SCENE_NONE;

	testFont = std::make_unique<UIFont>();
	testFont->Init("Assets/Texture/UI/ASCIILib_StringLiteral.png", { 20.f,45.3f});
	testFont->SetFontRectWidth(400);

	testFont->UpdateContents("This is another Test which is harder");
	
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
	//シーンの切り替え//todo can be automatic
	std::shared_ptr<UIButton> startButton = GetObj<UIButton>("title_start");
	if (startButton->isTrigger())
		mSceneIndex = SCENE_MAIN;

	std::shared_ptr<UIButton> exitButton = GetObj<UIButton>("title_exit");
	if (exitButton->isTrigger())
		mSceneIndex = EXIT;

	if (mSceneIndex != SCENE_NONE)
		ChangeScene();
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
		if (uiManager.contains(category))
		{
			for (std::shared_ptr<CanvasUI> obj : uiManager[category])
			{
				obj->Update(dt);
			}
		}
	}

	testFont->Update();
	GetObj<PBRModel>("testModel")->Update(dt);
}

void SceneTitle::Draw()
{
	GetObj<PBRModel>("testModel")->Draw();
	testFont->Draw();
	float currentZ = UI_FARZ - 0.1f;



	//for (const std::string& category : mUiOrder)
	//{
	//	// 获取该类别下的所有 UI 对象
	//	if (uiManager.contains(category))
	//	{
	//		for (std::shared_ptr<CanvasUI> obj : uiManager[category])
	//		{
	//			obj->SetPosZ(currentZ);
	//			obj->Draw();          
	//			currentZ -= 0.1f;      
	//		}
	//	}
	//}
	
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

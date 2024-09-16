#include <nlohmann/json.hpp>
#include <fstream>
#include "SceneTitle.h"
#include "FirstPersonCamera.h"
#include "CanvasUI.h"
#include "DirLight.h"
#include "SceneBlank.h"
#include "SceneManager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

enum TitleScene
{
	SCENE_MAIN,
	SCENE_OPTION,
	EXIT,
};

void SceneTitle::Init()
{
	//Setting
	CanvasUI* uiBg = CreateObj<CanvasUI>("title_bg");
	uiBg->InitResource("Assets/Texture/UI/main_title_background_1920x1080.png");

	CanvasUI* uiStart = CreateObj<UI_Button>("title_start");
	uiStart->InitResource("Assets/Texture/UI/main_title_startbutton_700x140.png");
	uiStart->InitData(Vector3(0, -20, 0.2f));

	CanvasUI* uiOption = CreateObj<UI_Button>("title_option");
	uiOption->InitResource("Assets/Texture/UI/main_title_optionbutton_700x140.png");
	uiOption->InitData(Vector3(0, -150, 0.2f));


	CanvasUI* uiExit = CreateObj<UI_Button>("title_exit");
	uiExit->InitResource("Assets/Texture/UI/main_title_exitbutton_700x140.png");
	uiExit->InitData(Vector3(0, -280, 0.2f));

	uiManager["background"].push_back(uiBg);  
	uiManager["button"].push_back(uiStart);   
	uiManager["button"].push_back(uiOption);  
	uiManager["button"].push_back(uiExit);

	mUiOrder = { "background",  "button" ,"effect" };
}

void SceneTitle::UnInit()
{
	
}

void SceneTitle::Update(float dt)
{
	for (const std::string& category : mUiOrder)
	{
		if (uiManager.contains(category))
		{
			for (CanvasUI* obj : uiManager[category])  // 使用引用
			{
				obj->Update(dt);
			}
		}
	}

	UI_Button* startButton = GetObj<UI_Button>("title_start");

	if(startButton->isPressed())
	{
		ChangeScene();
	}
	

}

void SceneTitle::Draw()
{
	float currentZ = UI_FARZ - 0.1f;
	for (const std::string& category : mUiOrder)
	{
		// 获取该类别下的所有 UI 对象
		if (uiManager.find(category) != uiManager.end())
		{
			
			for (CanvasUI* obj : uiManager[category])
			{
				obj->SetPosZ(currentZ);
				obj->Draw();           // 绘制对象
				currentZ -= 0.1f;      // 增加 z 值，保证后绘制的对象在前面
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
		//todo:insert scene_option here
	case EXIT:
		DebugLog::Log("Exiting the application...");
		PostQuitMessage(0); 
		break;
	}
}

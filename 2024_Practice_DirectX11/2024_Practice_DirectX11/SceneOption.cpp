#include "SceneOption.h"
#include <memory>
#include "D2DFont.h"
#include "D2D_UIRoundedRect.h"
#include "KInput.h"
#include "SceneManager.h"




void SceneOption::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_option.json");

	testButton = std::make_unique<D2D_UIStackContainer>();
	testButton->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "Test");
	testButton->LoadSaveData(sceneData["test"]);

	mButton = std::make_unique<UIButton>();
	mButton->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "button");
	mButton->LoadSaveData(sceneData["button"]);
	

}

void SceneOption::UnInit()
{

	json sceneData;
	sceneData["test"] = testButton->SaveData();
	sceneData["button"] = mButton->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_option.json", sceneData);


	
}

void SceneOption::Update(float dt)
{
	if(KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetMainScene("Title");//Back to TitleScene
	}

	//testButton->Update(dt);
	mButton->Update(dt);

}

void SceneOption::Draw()
{
	mButton->Draw();
	//testButton->Draw();
}

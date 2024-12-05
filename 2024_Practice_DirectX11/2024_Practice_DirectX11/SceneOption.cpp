#include "SceneOption.h"
#include <memory>
#include "KInput.h"
#include "SceneManager.h"
#include "UI_Square.h"


void SceneOption::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_option.json");
	container = std::make_unique<UIStackContainer>();
	container->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	container->LoadBackgroundTex(GetObj<Texture>("paper"), { 200,50 });
	container->LoadFontTexture(GetObj<Texture>("UIFont_Courier_New_It"), UITextOption::fontDefaultSize);
	container->SetObjName("container");
	container->LoadSaveData(sceneData, "test");
}

void SceneOption::UnInit()
{

	json sceneData;
	sceneData["test"] = container->SaveData("test");
	SaveSceneFile("Assets/Data/SaveDat/scene_option.json", sceneData);
}

void SceneOption::Update(float dt)
{

	if (KInput::IsKeyTrigger(VK_ESCAPE))
		SceneManager::Get()->SetMainScene("Title");

	container->Update();
	
}

void SceneOption::Draw()
{

	container->Draw();
	
}

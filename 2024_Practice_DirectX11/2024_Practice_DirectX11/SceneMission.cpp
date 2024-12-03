#include "SceneMission.h"

#include "GampApp.h"
#include "KInput.h"
#include "RenderState.h"

void SceneMission::Init()
{
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_mission.json");
	testContainer = std::make_unique<UIStackContainer>();
	testContainer->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	testContainer->LoadBackgroundTex(GetObj<Texture>("paper"),{200,50});
	testContainer->LoadFontTexture(GetObj<Texture>("UIFont_Courier_New_It"), UITextOption::fontDefaultSize);

	testContainer->LoadSaveData(sceneData,"Test");

}

void SceneMission::UnInit()
{
	json sceneData;

	sceneData["Test"] = testContainer->SaveData("Test");
	SaveSceneFile("Assets/Data/SaveDat/scene_mission.json", sceneData);
	
}

void SceneMission::Update(float dt)
{
	testContainer->Update();

	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		return;
	}

}

void SceneMission::Draw()
{
	
	testContainer->Draw();

}

#include "SceneMission.h"

#include "GampApp.h"
#include "KInput.h"
#include "RenderState.h"

void SceneMission::Init()
{
	testContainer = std::make_unique<UIStackContainer>();
	testContainer->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE);
	testContainer->LoadBackgroundTex(GetObj<Texture>("paper"),{200,50});
	testContainer->LoadFontTexture(GetObj<Texture>("UIFont_Courier_New_Regular"), UITextOption::fontDefaultSize);


	testContainer->SetText("So this is another test");

	testContainer->InitPosition(0, 0);
	testContainer->SetPosZ(0.4f);

}

void SceneMission::UnInit()
{
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

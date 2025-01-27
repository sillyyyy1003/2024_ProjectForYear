#include "SceneOption.h"
#include <memory>
#include "D2D_UIRoundedRect.h"
#include "GameApp.h"
#include "KInput.h"
#include "MissionManager.h"
#include "RandomGenerator.h"
#include "RenderState.h"
#include "ResultManager.h"
#include "SceneManager.h"
#include "ScreenFadeEffect.h"

void SceneOption::Init()
{
	test = std::make_unique<UI_IconButton>();
	//test->Init(D2DUIConfig::UIShape::ROUNDED_RECT, "Assets/Texture/ButtonUI/chargeTex.png", "Test");
	test->Init("Assets/Texture/ButtonUI/IconBg.dds", "Assets/Texture/ButtonUI/chargeTex.dds", "Test");

}

void SceneOption::UnInit()
{


}

void SceneOption::Update(float dt)
{
	if(KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetMainScene("Title");//Back to TitleScene
	}

	if(KInput::IsKeyTrigger(VK_SPACE))
	{
		ResultManager::Get()->SetActive(true);
	}


	MissionManager::Get()->MissionGenerator();
	
	test->Update(dt);
}

void SceneOption::Draw()
{
	test->Draw();

}

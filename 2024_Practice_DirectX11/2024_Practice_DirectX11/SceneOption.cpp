#include "SceneOption.h"
#include <memory>
#include "KInput.h"
#include "SceneManager.h"
#include "UI_Square.h"


void SceneOption::Init()
{
	test = std::make_shared<UI_Square>();
	test->Init(nullptr,20);
	test->LoadDefShader("Assets/Shader/VS_DefaultUI.cso", "Assets/Shader/PS_DefaultUI.cso");
	test->SetScale({100,100});
}

void SceneOption::UnInit()
{
}

void SceneOption::Update(float dt)
{
	if (KInput::IsKeyTrigger(VK_ESCAPE))
		SceneManager::Get()->SetMainScene("Title");

	test->Update();
	
}

void SceneOption::Draw()
{
	test->Draw();

}

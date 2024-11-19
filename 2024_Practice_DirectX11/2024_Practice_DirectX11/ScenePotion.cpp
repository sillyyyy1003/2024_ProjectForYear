#include "ScenePotion.h"

#include "KInput.h"
#include "SceneManager.h"

void ScenePotion::Init()
{

}

void ScenePotion::UnInit()
{
}

void ScenePotion::Update(float dt)
{
	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Lab");
		return;
	}
}

void ScenePotion::Draw()
{
}

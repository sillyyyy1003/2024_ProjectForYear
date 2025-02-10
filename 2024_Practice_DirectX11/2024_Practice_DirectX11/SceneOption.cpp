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
	mTest = std::make_unique<FireEffect>();
	mTest->InitParticleRenderer(1000,0.01f);
	mTest->InitFireParticleData({}, Vector3(0.15f, 0.5f, 0.15f), Vector3(0, -0.1f, 0),2.f);
	mTest->SetParticleColorRange(Color(1, 0.2f, 0, 1), Color(1, 0, 0, 0.0f));
	

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


	MissionManager::Get()->MissionGenerator();
	
	mTest->Update(dt);
}

void SceneOption::Draw()
{
	GameApp::SetBlendState(RenderState::BSAlphaWeightedAdditive);
	mTest->Draw();
	GameApp::SetBlendState(nullptr);
}

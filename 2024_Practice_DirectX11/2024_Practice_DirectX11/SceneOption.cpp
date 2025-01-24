#include "SceneOption.h"
#include <memory>
#include "D2D_UIRoundedRect.h"
#include "GameApp.h"
#include "KInput.h"
#include "MissionManager.h"
#include "RandomGenerator.h"
#include "RenderState.h"
#include "SceneManager.h"
#include "ScreenFadeEffect.h"

void SceneOption::Init()
{


	//testParticle = std::make_unique<ParticleEffect>();
	//testParticle->InitParticleRenderer("VS_ParticleInstance.hlsl",2000);
	//testParticle->InitPointOnCircleParticle({ 0,0,0 }, 2.0, { 0,0.1f,0 }, { 0,0.1f,0 });
	//testParticle->SetParticleAliveTime(2.0f);
	//testParticle->SetEffectAliveTime(10.f);
	//testParticle->InitRandomParticleColor({9,60});
	//testParticle->SetLoop(false);
	//testParticle = std::make_unique<FireEffect>();
	//testParticle->InitParticleRenderer("VS_ParticleInstance.hlsl", 10, 1.f);
	////testParticle->InitPointOnCircleParticle({ 0.0,0.0,0.0 }, 0.2f, { 0.0,0.0,0.0 }, Velocity);
	//testParticle->InitParticleColor();
	//testParticle->InitFireParticleData({ 0,0,0 }, { 0.0,-0.1f,0.0f }, { 0,1.f,0 });
	//testParticle->SetParticleAliveTime(5.f);
	//testParticle->SetScale({1.f, 10.f, 1.f});
	/*testParticle->SetPotion(this);*/



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

	}


	MissionManager::Get()->MissionGenerator();
	/*testParticle->Update(dt);*/
;

}

void SceneOption::Draw()
{


	GameApp::SetBlendState(RenderState::BSTransparent);
	//testParticle->Draw();
	GameApp::SetBlendState(nullptr);

}

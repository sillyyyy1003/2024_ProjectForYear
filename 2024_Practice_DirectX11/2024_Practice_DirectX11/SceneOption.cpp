#define _CRT_SECURE_NO_WARNINGS
#include "SceneOption.h"
#include <memory>
#include "D2D_UIRoundedRect.h"
#include "GameApp.h"
#include "KInput.h"
#include "MissionManager.h"
#include "SceneManager.h"

void SceneOption::Init()
{
	PixelShader* ps = CreateObj<PixelShader>("psi").get();
	ps->LoadShaderFile("Assets/Shader/PS_ParticleInstance.cso");

	testParticle = std::make_unique<ParticleEffect>();
	testParticle->InitParticleRenderer("VS_ParticleInstance.hlsl",1000);
	testParticle->InitParticleData({2,0,0},{0,3,0},{1,1,1},4);
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
	testParticle->Update(dt);

}

void SceneOption::Draw()
{
	PixelShader* ps = GetObj<PixelShader>("psi").get();
	ps->SetShader();
	testParticle->Draw();


}

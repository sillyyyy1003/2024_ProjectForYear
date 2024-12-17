#include "ScreenOverlay.h"
#include "GameApp.h"
#include "RenderState.h"
#include "UI_Square.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;
ScreenOverlay::ScreenOverlay()
{
}

ScreenOverlay::~ScreenOverlay()
{
}

void ScreenOverlay::Init()
{
	mOverlay = std::make_unique<UI_Square>();
	mOverlay->Init(SceneManager::Get()->GetObj<Texture>("BlackOverlay"), 0, XMINT2(1,1));
	mOverlay->LoadDefShader();
	mOverlay->SetScale(2000.f, 2000.f);
	mOverlay->SetPosZ(2.9f);
	mOverlay->SetTransparency(0.45f);
}

void ScreenOverlay::Update(float dt)
{
	mOverlay->Update();
}

void ScreenOverlay::ScreenOverlayEffect()
{
	GameApp::SetBlendState(RenderState::BSTransparent);
	mOverlay->Draw();
	GameApp::SetBlendState(nullptr);
}

void ScreenOverlay::SetPosZ(float posZ)
{
	mOverlay->SetPosZ(posZ);
}


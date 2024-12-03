#include "ScreenOverlay.h"
#include "GampApp.h"
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
	mOverlay->Init(GameApp::GetComponent<Texture>("BlackOverlay"), 0, XMINT2(1,1));
	mOverlay->SetScale((float)(GameApp::Get()->GetWinWidth()), (float)GameApp::Get()->GetWinHeight());


}

void ScreenOverlay::Update()
{
	mOverlay->Update();
}

void ScreenOverlay::ScreenOverlayEffect()
{
}


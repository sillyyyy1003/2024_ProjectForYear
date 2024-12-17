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

	mFade = std::make_unique<UI_Square>();
	mFade->Init(SceneManager::Get()->GetObj<Texture>("Fade"), 0, XMINT2(1, 1));
	mFade->LoadDefShader();
	mFade->SetScale(2000.f, 2000.f);
	mFade->SetPosZ(0.1f);
	mFade->SetTransparency(0.0f);

}

void ScreenOverlay::Update(float dt)
{
	switch (mState)
	{
	default:
	case ScreenOverlayConfig::STATE_SCREEN_OVERLAY:
		break;
	case ScreenOverlayConfig::STATE_NONE:
		break;
	case ScreenOverlayConfig::STATE_FADE_OUT:
		FadeOut(dt);
		break;
	case ScreenOverlayConfig::STATE_FADE_IN:
		FadeIn(dt);
		break;
	}


	mOverlay->Update();
}

void ScreenOverlay::SetDefaultState()
{
	mState = ScreenOverlayConfig::STATE_NONE;
}

void ScreenOverlay::ScreenOverlayEffect()
{
	mState = ScreenOverlayConfig::STATE_SCREEN_OVERLAY;
}

void ScreenOverlay::SetState(ScreenOverlayConfig::OverlayState state)
{
	mState = state;
}

void ScreenOverlay::SetPosZ(float posZ)
{
	mOverlay->SetPosZ(posZ);
}

void ScreenOverlay::Draw()
{
	switch(mState)
	{
		default:
	case ScreenOverlayConfig::STATE_SCREEN_OVERLAY:
		GameApp::SetBlendState(RenderState::BSMulti);
		mOverlay->Draw();
		break;
	case ScreenOverlayConfig::STATE_NONE:
		break;
	case ScreenOverlayConfig::STATE_FADE_OUT:
		mFade->Draw();
		break;
	case ScreenOverlayConfig::STATE_FADE_IN:
		mFade->Draw();
		break;
	}

}

void ScreenOverlay::FadeOut(float dt)
{
	if (mFadeParam < 1.0f)
		mFadeParam += ScreenOverlayConfig::FADE_SPEED * dt;
	else
		mState = ScreenOverlayConfig::STATE_FADE_IN;

	mFade->SetTransparency(mFadeParam);
}

void ScreenOverlay::FadeIn(float dt)
{
	if (mFadeParam > 0.0f)
		mFadeParam -= ScreenOverlayConfig::FADE_SPEED * dt;
	else
		mState = ScreenOverlayConfig::STATE_NONE;

	mFade->SetTransparency(mFadeParam);
}


bool ScreenOverlay::GetFadeOut()
{
	if (mState == ScreenOverlayConfig::STATE_FADE_OUT)return true;

	return false;
}


bool ScreenOverlay::GetFadeIn()
{
	if (mState == ScreenOverlayConfig::STATE_FADE_IN)return true;
	return false;
}

bool ScreenOverlay::GetFade()
{
	if (mState == ScreenOverlayConfig::STATE_FADE_IN || mState == ScreenOverlayConfig::STATE_FADE_OUT)
		return true;

	return false;
}




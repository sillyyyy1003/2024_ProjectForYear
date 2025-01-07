#include "ScreenFadeEffect.h"
#include "GameApp.h"
#include "RenderState.h"
#include "UI_Square.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


ScreenFadeEffect::ScreenFadeEffect()
{
}

ScreenFadeEffect::~ScreenFadeEffect()
{
}

void ScreenFadeEffect::Init()
{
	mFade = std::make_unique<UI_Square>();
	mFade->Init(SceneManager::Get()->GetObj<Texture>("Fade"), 0, XMINT2(1, 1));
	mFade->LoadDefShader();
	mFade->SetScale(2000.f, 2000.f);
	mFade->SetPosZ(0.1f);
	mFade->SetTransparency(0.0f);
}

void ScreenFadeEffect::Update(float dt)
{
	switch (mState)
	{
	default:

	case ScreenOverlayConfig::STATE_NONE:
		break;
	case ScreenOverlayConfig::STATE_FADE_OUT:
		FadeOut(dt);
		break;
	case ScreenOverlayConfig::STATE_FADE_IN:
		FadeIn(dt);
		break;
	}


}

void ScreenFadeEffect::SetDefaultState()
{
	mState = ScreenOverlayConfig::STATE_NONE;
}


void ScreenFadeEffect::SetState(ScreenOverlayConfig::OverlayState state)
{
	mState = state;
}



void ScreenFadeEffect::Draw()
{
	switch(mState)
	{
		default:
	case ScreenOverlayConfig::STATE_NONE:
		break;
	case ScreenOverlayConfig::STATE_FADE_OUT:
		mFade->Draw();
		break;
	case ScreenOverlayConfig::STATE_FADE_IN:
		GameApp::SetBlendState(RenderState::BSTransparent);
		mFade->Draw();
		break;
	}

}

void ScreenFadeEffect::FadeOut(float dt)
{
	if (mFadeParam < 1.0f)
		mFadeParam += ScreenOverlayConfig::FADE_SPEED * dt;
	else
		mState = ScreenOverlayConfig::STATE_FADE_IN;

	mFade->SetTransparency(mFadeParam);
}

void ScreenFadeEffect::FadeIn(float dt)
{
	if (mFadeParam > 0.0f)
		mFadeParam -= ScreenOverlayConfig::FADE_SPEED * dt;
	else
		mState = ScreenOverlayConfig::STATE_NONE;

	mFade->SetTransparency(mFadeParam);
}


bool ScreenFadeEffect::GetFadeOut()
{
	if (mState == ScreenOverlayConfig::STATE_FADE_OUT)return true;

	return false;
}


bool ScreenFadeEffect::GetFadeIn()
{
	if (mState == ScreenOverlayConfig::STATE_FADE_IN)return true;
	return false;
}

bool ScreenFadeEffect::GetFade()
{
	if (mState == ScreenOverlayConfig::STATE_FADE_IN || mState == ScreenOverlayConfig::STATE_FADE_OUT)
		return true;

	return false;
}




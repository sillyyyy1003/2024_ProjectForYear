#include "ScreenFadeEffect.h"
#include "GameApp.h"
#include "MovableStaticObject.h"
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

	mWhite = std::make_unique<UI_Square>();
	mWhite->Init(SceneManager::Get()->GetObj<Texture>("WhiteGradation"), 10, XMINT2(1, 1));
	mWhite->LoadDefShader();
	mWhite->SetScale(100.f, 100.f);
	mWhite->SetPosZ(0.1f);
	
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
	case ScreenOverlayConfig::STATE_WHITE_OUT:
		WhiteOut(dt);
		break;
	case ScreenOverlayConfig::STATE_WHITE_IN:
		WhiteIn(dt);
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

void ScreenFadeEffect::SetWhiteDuration(float outDuration, float inDuration)
{
	mOutDuration = outDuration;
	mInDuration = inDuration;
}

void ScreenFadeEffect::Draw()
{
	switch(mState)
	{
		default:
	case ScreenOverlayConfig::STATE_NONE:
		break;
	case ScreenOverlayConfig::STATE_FADE_OUT:
		GameApp::SetBlendState(RenderState::BSTransparent);
		mFade->Draw();
		GameApp::SetBlendState(nullptr);
		break;
	case ScreenOverlayConfig::STATE_FADE_IN:
		GameApp::SetBlendState(RenderState::BSTransparent);
		mFade->Draw();
		GameApp::SetBlendState(nullptr);
		break;
	case ScreenOverlayConfig::STATE_WHITE_OUT:
		GameApp::SetBlendState(RenderState::BSTransparent);
		mWhite->Draw();
		GameApp::SetBlendState(nullptr);
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

void ScreenFadeEffect::WhiteOut(float dt)
{
	if (mAccumulateTime <= mOutDuration)
	{
		float whiteRatio = mAccumulateTime / mOutDuration;
		whiteRatio = EaseOut::EaseInCubic(whiteRatio);
		mWhite->SetScale(Vector2(3500, 3500) * whiteRatio);
		mAccumulateTime += dt;
		mWhite->SetTransparency(whiteRatio);
	}
	else
	{
		mAccumulateTime = 0.f;
		mCBuffer.radius = 0.0f;
		mState = ScreenOverlayConfig::STATE_NONE;
	}
}

void ScreenFadeEffect::WhiteIn(float dt)
{
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




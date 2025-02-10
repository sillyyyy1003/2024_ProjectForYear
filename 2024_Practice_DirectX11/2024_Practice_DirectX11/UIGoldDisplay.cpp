#include "UIGoldDisplay.h"

#include "MovableStaticObject.h"
#include "Player.h"

enum GoldDisplayUIState
{
	STATE_NONE,
	STATE_INCREASE,
	STATE_MAX,

};

void UIGoldDisplay::Init(Player* player, D2DUIConfig::FontSize fontSize)
{
	mPlayer = player;
	mGoldUI = std::make_unique<D2D_UIStackContainer>();
	mGoldUI->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "GoldDisplay");
	mGoldUI->SetUIState(D2DUIConfig::STATE_USE_FONT);
}

void UIGoldDisplay::Update(float dt)
{

	switch (mState)
	{
		default:
	case STATE_NONE:
		OnStateNone(dt);
		break;
	case STATE_INCREASE:
		OnStateCoinNumChange(dt);
		break;
	}
	mGoldUI->Update(dt);
}

void UIGoldDisplay::Draw()
{
	mGoldUI->Draw();
}

void UIGoldDisplay::LoadSceneData(const json& data)
{
	mGoldUI->LoadSaveData(data);
}

json UIGoldDisplay::SaveData()
{
	return mGoldUI->SaveData();
}

void UIGoldDisplay::SetIncreaseFunction(float duration, int increaseNum)
{
	mState = STATE_INCREASE;

	mDuration = duration;
	mIncreaseNum = increaseNum;
	mStartNum = mPlayer->GetPlayerGoldNum();
	mIncreaseResult = mIncreaseNum + mStartNum;
	mPlayer->SetGold(mIncreaseResult);
}

void UIGoldDisplay::OnStateNone(float dt)
{
	//Œ»Ý‚ÌƒRƒCƒ“”‚ð•\Ž¦
	mGoldUI->SetText(mPlayer->GetPlayerGold().c_str());
}

void UIGoldDisplay::OnStateCoinNumChange(float dt)
{
	if (mAccumulateTime <= mDuration)
	{
		float currentIncreaseNum = mIncreaseNum * (Ease::EaseOutCubic(mAccumulateTime / mDuration));
		mAccumulateTime += dt;
		//Œ»Ý‚Ì”Žš‚ðŽæ“¾
		std::string str= std::to_string((int)(mStartNum + currentIncreaseNum));
		mGoldUI->SetText(str.c_str());
	}else
	{
		mAccumulateTime = 0.f;
		mGoldUI->SetText(mPlayer->GetPlayerGold().c_str());
		mState = STATE_NONE;
	}


}

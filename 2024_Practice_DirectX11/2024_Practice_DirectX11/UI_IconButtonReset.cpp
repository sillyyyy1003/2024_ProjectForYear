#include "UI_IconButtonReset.h"
#include "Potion.h"


void UI_IconButtonReset::OnStateNone(float dt)
{
	//位置設定
	UI_IconButton::OnStateNone(dt);
	//色変更
	mBg->SetDiffuseColor(ColorConfig::WHITE);
	mButtonIcon->SetDiffuseColor(ColorConfig::BLACK);

}

void UI_IconButtonReset::OnStateTrigger(float dt)
{
	//位置設定
	UI_IconButton::OnStateTrigger(dt);
	//色変更
	mBg->SetDiffuseColor(ColorConfig::BLACK);
	mButtonIcon->SetDiffuseColor(ColorConfig::WHITE);

	//Button Function
	mPotion->ResetPotion();
}

void UI_IconButtonReset::OnStateHover(float dt)
{
	//位置設定
	UI_IconButton::OnStateHover(dt);
	//色変更
	mBg->SetDiffuseColor(ColorConfig::BLACK);
	mButtonIcon->SetDiffuseColor(ColorConfig::WHITE);
}

void UI_IconButtonReset::OnStatePress(float dt)
{
	//位置設定
	UI_IconButton::OnStatePress(dt);
	//色変更
	mBg->SetDiffuseColor(ColorConfig::BLACK);
	mButtonIcon->SetDiffuseColor(ColorConfig::WHITE);
}

void UI_IconButtonReset::SetPotion(Potion* potion)
{
	mPotion = potion;
}

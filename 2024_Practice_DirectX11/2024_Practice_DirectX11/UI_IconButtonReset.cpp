#include "UI_IconButtonReset.h"
#include "Potion.h"


void UI_IconButtonReset::OnStateNone(float dt)
{
	//�ʒu�ݒ�
	UI_IconButton::OnStateNone(dt);
	//�F�ύX
	mBg->SetDiffuseColor(ColorConfig::WHITE);
	mButtonIcon->SetDiffuseColor(ColorConfig::BLACK);

}

void UI_IconButtonReset::OnStateTrigger(float dt)
{
	//�ʒu�ݒ�
	UI_IconButton::OnStateTrigger(dt);
	//�F�ύX
	mBg->SetDiffuseColor(ColorConfig::BLACK);
	mButtonIcon->SetDiffuseColor(ColorConfig::WHITE);

	//Button Function
	mPotion->ResetPotion();
}

void UI_IconButtonReset::OnStateHover(float dt)
{
	//�ʒu�ݒ�
	UI_IconButton::OnStateHover(dt);
	//�F�ύX
	mBg->SetDiffuseColor(ColorConfig::BLACK);
	mButtonIcon->SetDiffuseColor(ColorConfig::WHITE);
}

void UI_IconButtonReset::OnStatePress(float dt)
{
	//�ʒu�ݒ�
	UI_IconButton::OnStatePress(dt);
	//�F�ύX
	mBg->SetDiffuseColor(ColorConfig::BLACK);
	mButtonIcon->SetDiffuseColor(ColorConfig::WHITE);
}

void UI_IconButtonReset::SetPotion(Potion* potion)
{
	mPotion = potion;
}

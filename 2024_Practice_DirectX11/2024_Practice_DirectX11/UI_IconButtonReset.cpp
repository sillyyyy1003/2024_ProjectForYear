#include "UI_IconButtonReset.h"

#include "Potion.h"
#include "SceneManager.h"

void UI_IconButtonReset::OnStateNone(float dt)
{
	//位置設定
	UI_IconButton::OnStateNone(dt);
	//色変更
	mBg->SetDiffuseColor(ColorConfig::WHITE);
	mButtonIcon->SetDiffuseColor(ColorConfig::BLACK);


	//水がもうすぐ溢れる場合

}

void UI_IconButtonReset::OnStateTrigger(float dt)
{
	//位置設定
	UI_IconButton::OnStateTrigger(dt);
	//色変更
	mBg->SetDiffuseColor(ColorConfig::BLACK);
	mButtonIcon->SetDiffuseColor(ColorConfig::WHITE);

	//Button Function
	SceneManager::Get()->GetObj<Potion>("PotionSceneWater")->ResetWaterLevel();
	SceneManager::Get()->GetObj<Potion>("PotionSceneWater")->ResetMaterial();


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

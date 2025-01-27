#include "UI_IngredientIconButton.h"
#include "GameApp.h"
#include "RenderState.h"

using namespace DirectX::SimpleMath;

void UI_IngredientIconButton::Init(const char* bgTex, const char* iconTex, const char* objName)
{
	UI_IconButton::Init(bgTex, iconTex, objName);
	mCapacity = std::make_unique<UI_Square>();
	mCapacity->Init(bgTex,10);
	mCapacity->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_DefaultUI"), SceneManager::Get()->GetObj<PixelShader>("PS_IconButtonIngredientCapacity"));

	mCapacity->SetPosZ(0.8f);//キャパシティはアイコンの下　背景の上
}


void UI_IngredientIconButton::OnStateNone(float dt)
{
	//位置
	UI_IconButton::OnStateNone(dt);
#ifdef _DEBUG
	if (mIngredient == nullptr)
	{
		DebugLog::LogError("Ingredient is null!");
		return;
	}
#endif
	//色
	HSV color = {};
	color.hue = RGBtoHSV(mIngredient->GetColor()).hue;
	mBg->SetDiffuseColor(ColorConfig::WHITE);
	mCapacity->SetDiffuseColor(HSVtoRGB(color));
	mButtonIcon->SetDiffuseColor(ColorConfig::BLACK);
}

void UI_IngredientIconButton::OnStateHover(float dt)
{
	UI_IconButton::OnStateHover(dt);

	HSV color = {};
	color.hue = RGBtoHSV(mIngredient->GetColor()).hue;
	mButtonIcon->SetDiffuseColor(HSVtoRGB(color));
	mBg->SetDiffuseColor(ColorConfig::WHITE);
}

void UI_IngredientIconButton::OnStatePress(float dt)
{
	UI_IconButton::OnStatePress(dt);
	HSV color = {};
	color.hue = RGBtoHSV(mIngredient->GetColor()).hue;
	mButtonIcon->SetDiffuseColor(HSVtoRGB(color));
	mBg->SetDiffuseColor(ColorConfig::WHITE);
}

void UI_IngredientIconButton::OnStateTrigger(float dt)
{
	UI_IconButton::OnStateTrigger(dt);

	HSV color = {};
	color.hue = RGBtoHSV(mIngredient->GetColor()).hue;
	mButtonIcon->SetDiffuseColor(HSVtoRGB(color));
	mBg->SetDiffuseColor(ColorConfig::WHITE);

	//Charge Ingredient;
	mIngredient->ChargeCapacity();
}

void UI_IngredientIconButton::Draw()
{
	//アイコン描画
	GameApp::SetBlendState(RenderState::BSTransparent);


	//キャパシティ描画 Default 状態のみ
	if(mState== UIButtonConfig::STATE_NONE)
	{
		struct CBuffer
		{
			DirectX::XMFLOAT4 color;
			float capacity;
		};

		HSV color = {};
		color.hue = RGBtoHSV(mIngredient->GetColor()).hue;

		CBuffer cb = {
				{1,1,1,1},
				mIngredient->GetCapacityPercentage()
		};
		mCapacity->SetScale({ 0.9f * mBg->GetScale().x,0.9f * mBg->GetScale().y });//背景と同じテクスチャ使うがサイズはちょっと小さく
		mCapacity->SetPosition(mBg->GetPosition());
		mCapacity->GetDefPS()->WriteShader(1, &cb);
		mCapacity->Draw();
	}

	//ICON描画
	mButtonIcon->Draw();

	//影描画
	if (mState != UIButtonConfig::STATE_NONE)mShadow->Draw();

	//背景描画
	mBg->Draw();
	GameApp::SetBlendState(nullptr);
}

//void UI_IngredientIconButton::LoadSaveData(json data)
//{
//	UI_IconButton::LoadSaveData(data);
//
//}

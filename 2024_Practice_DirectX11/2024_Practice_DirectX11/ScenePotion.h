﻿#pragma once
#include "CandleLight.h"
#include "Ingredient.h"
#include "InteractiveStaticObject.h"
#include "NonDiegeticUI_PotTop.h"
#include "Potion.h"
#include "SceneBase.h"
#include "StaticObject.h"
#include "UIGoldDisplay.h"
#include "UI_IconButtonReset.h"
#include "UI_IngredientIconButton.h"
#include "WaterJug.h"


class ScenePotion :public SceneBase
{
	//シーン切り替え用
	SceneConfig::SceneIndex mNextScene = SceneConfig::SceneIndex::SCENE_NONE;

	//Texture List
	PBRConfig::PBRTexList pbrTexList;

	//Water
	std::shared_ptr<Potion> mWater;

	//Ingredient
	std::shared_ptr<Ingredient> mRedPotion;
	std::shared_ptr<Ingredient> mBluePotion;
	std::shared_ptr<Ingredient> mYellowPotion;
	//Pot
	std::shared_ptr<InteractiveStaticObject> mPot;
	std::unique_ptr<NonDiegeticUI_PotTop> mPotTop;
	//WaterJug
	std::shared_ptr<WaterJug> mJug;

	//MissionPaper
	std::unique_ptr<StaticObject> mPaper;
	std::unique_ptr<StaticObject> mSplash;

	//Table
	std::unique_ptr<StaticObject> mTable;

	//PointLight;
	std::unique_ptr<CandleLight> mCandleLight1;
	std::unique_ptr<CandleLight> mCandleLight2;

	//顔料補充ボタン
	std::unique_ptr<UI_IngredientIconButton> mRedIngredientButton;
	std::unique_ptr<UI_IngredientIconButton> mBlueIngredientButton;
	std::unique_ptr<UI_IngredientIconButton> mYellowIngredientButton;

	//鍋のリセットボタン
	std::unique_ptr<UI_IconButtonReset> mResetButton;

	std::shared_ptr<UIGoldDisplay> mGoldDisplay;


public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();

	void DrawWithShadow();
	void InitShadowRenderTarget();

private:
	void GameObjectUpdate(float dt);
	void TriggerListener();
};


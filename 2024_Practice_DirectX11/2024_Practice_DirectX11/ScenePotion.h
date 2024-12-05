#pragma once
#include "CandleLight.h"
#include "Ingredient.h"
#include "SceneBase.h"
#include "StaticObject.h"
#include "Water.h"

class ScenePotion :public SceneBase
{
	//Texture List
	PBRConfig::PBRTexList pbrTexList;

	//Water
	std::shared_ptr<Water> mWater;

	//Pot
	std::unique_ptr<StaticObject> mPot;
	//Table
	std::unique_ptr<StaticObject> mTable;

	//PointLight;
	std::unique_ptr<CandleLight> mCandleLight;

	std::unique_ptr<Ingredient> testIngredientRed;
	std::unique_ptr<Ingredient> testIngredientBlue;
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();

private:
	void GameObjectUpdate(float dt);
	void TriggerListener();
};


#pragma once
#include "CandleLight.h"
#include "SceneBase.h"
#include "StaticObject.h"
#include "Water.h"

class ScenePotion :public SceneBase
{
	//Texture List
	PBRConfig::PBRTexList pbrTexList;

	//Water
	std::unique_ptr<Water> mWater;

	//Pot
	std::unique_ptr<StaticObject> mPot;
	//Table
	std::unique_ptr<StaticObject> mTable;

	//PointLight;
	std::unique_ptr<CandleLight> mCandleLight;
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


#pragma once
#include "InteractiveStaticObject.h"
#include "SceneBase.h"
#include "Water.h"

class ScenePotion :public SceneBase
{
	//Texture List
	PBRConfig::PBRTexList pbrTexList;

	//Water
	std::unique_ptr<Water> mWater;

	//Pot
	std::unique_ptr<InteractiveStaticObject> mPot;

	
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


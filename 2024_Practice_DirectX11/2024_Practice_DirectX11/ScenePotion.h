#pragma once
#include "SceneBase.h"
#include "Water.h"

class ScenePotion :public SceneBase
{
	std::unique_ptr<Water> mWater;


public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


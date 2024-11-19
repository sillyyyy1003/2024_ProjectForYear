#pragma once
#include "SceneBase.h"
class ScenePotion :public SceneBase
{
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


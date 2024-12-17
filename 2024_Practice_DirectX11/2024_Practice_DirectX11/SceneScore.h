#pragma once
#include "UIStackContainer.h"

class SceneScore
{
	std::unique_ptr<UIStackContainer> mScoreBillboard;

public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


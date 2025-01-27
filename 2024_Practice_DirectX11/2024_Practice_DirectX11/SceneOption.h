#pragma once
#include "PBRModel.h"
#include "SceneBase.h"
#include "UI_IconButton.h"


class SceneOption : public SceneBase
{
	std::unique_ptr<UI_IconButton> test;
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


#pragma once
#include "D2D_UIRect.h"
#include "D2D_UIStackContainer.h"
#include "PBRModel.h"
#include "SceneBase.h"
#include "UIButton.h"


class SceneOption : public SceneBase
{
	std::unique_ptr<D2D_UIBase> test;
	std::unique_ptr<D2D_UIStackContainer> testButton;
	std::unique_ptr<UIButton> mButton;
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


#pragma once
#include "UI_IconButton.h"

/// @brief 鍋の水の色と量をリセット
class UI_IconButtonReset:public UI_IconButton
{
public:
	void OnStateNone(float dt) override;
	void OnStateTrigger(float dt) override;
	void OnStateHover(float dt) override;
	void OnStatePress(float dt) override;
};


#pragma once
#include "Potion.h"
#include "UI_IconButton.h"

/// @brief ��̐��̐F�Ɨʂ����Z�b�g
class UI_IconButtonReset:public UI_IconButton
{
	Potion* mPotion = nullptr;
	float mAccumulateTime = 0.0f;

public:
	void OnStateNone(float dt) override;
	void OnStateTrigger(float dt) override;
	void OnStateHover(float dt) override;
	void OnStatePress(float dt) override;

	void SetPotion(Potion* potion);
};


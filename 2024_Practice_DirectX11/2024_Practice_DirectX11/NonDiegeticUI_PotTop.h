#pragma once
#include "InteractiveStaticObject.h"
class NonDiegeticUI_PotTop :public InteractiveStaticObject
{
public:
	NonDiegeticUI_PotTop();
	~NonDiegeticUI_PotTop() override = default;

	void PreUpdate(float dt) override;
	void GameUpdate(float dt) override;

	void OnStateClicked() override;
	void OnStateHover() override;
	void OnStateNone() override;
	void OnStateResult();
};


#pragma once
#include "D2D_UIStackContainer.h"

class UIButton :public D2D_UIStackContainer
{
private:
	RECT mRect = {};
	int mState = 0;

public:
	UIButton();
	~UIButton() = default;

	void Init(D2DUIConfig::UIShape shape, D2DUIConfig::FontSize size, const char* objName) override;

	void Update(float dt) override;

	void UpdateCollider();

	void PreUpdate(float dt);
	void GameUpdate(float dt);
	void LateUpdate(float dt);

	bool IsPressed();
	bool IsTrigger();
};


#pragma once
#include "D2D_UIStackContainer.h"


class UIButton :public D2D_UIStackContainer
{
private:

	RECT mRect = {};
	int mState = 0;

	D2D1::ColorF mDefaultColor = { 1.f,1.f,1.f,1.f };
	D2D1::ColorF mDefaultFontColor = { 1.f,1.f,1.f,1.f };

public:
	UIButton();
	~UIButton() override = default;

	virtual void Init(D2DUIConfig::UIShape shape, D2DUIConfig::FontSize size, const char* objName) override;

	virtual void Update(float dt) override;
	virtual void UpdateCollider();

	virtual void PreUpdate(float dt);
	virtual void GameUpdate(float dt);
	virtual void LateUpdate(float dt);

	virtual void LoadSaveData(json data) override;
	virtual json SaveData() override;

	virtual bool IsPressed();
	virtual bool IsTrigger();

	/// @brief 浮遊動作
	virtual void Levitate();

	/// @brief 元の色を設定する
	/// @param color 
	virtual void SetDefaultColor(D2D1::ColorF color);
};


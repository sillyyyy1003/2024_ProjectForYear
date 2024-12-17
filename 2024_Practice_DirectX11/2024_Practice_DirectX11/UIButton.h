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

	void Init(D2DUIConfig::UIShape shape, D2DUIConfig::FontSize size, const char* objName) override;

	void Update(float dt) override;

	void UpdateCollider();

	void PreUpdate(float dt);
	void GameUpdate(float dt);
	void LateUpdate(float dt);

	void LoadSaveData(json data) override;
	json SaveData() override;


	bool IsPressed();
	bool IsTrigger();

	/// @brief ïÇóVìÆçÏ
	void Levitate();

	void SetDefaultColor(D2D1::ColorF color);
};


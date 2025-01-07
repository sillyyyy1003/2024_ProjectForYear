#pragma once
#include "Water.h"

class Potion :public Water
{
private:

	float mDiluteParam = 0.2f;//水的稀释程度
	float mAccumulateTime = 0.0f;

	bool isAutoColor = false; //自動色変換
	float mColorTransformSpeed = 75.f;
public:

	Potion() = default;
	~Potion() override = default;


	void Update(float dt) override;

	//Dilute by Water
	void Dilute();
	void Dilute(float diluteAlpha);

	void MixColor(DirectX::XMFLOAT4 color,float alpha);
	HSV BlendColor(const HSV& color1, const HSV& color2,float alpha);

	void AutoColor(float dt);

	void SetAutoColor(bool isAutoColour) { this->isAutoColor = isAutoColour; };
	void SetWaterColor(DirectX::XMFLOAT4 color);


};


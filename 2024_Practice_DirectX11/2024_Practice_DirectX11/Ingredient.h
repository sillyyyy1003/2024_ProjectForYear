#pragma once
#include "InteractiveMovableObject.h"
#include "Potion.h"

class Ingredient:public InteractiveMovableObject
{
	struct ContainerData
	{
		float capacityHeight;
		DirectX::XMFLOAT3 pigmentColor;
	};
private:

	float mAccumulateTime = 0.0f;//Counting pouring time
	float mCapacity = 90.f;
	float mAlpha = 0.025f;		//顔料の影響係数


	//Default状態の時の位置
	DirectX::XMFLOAT3 mDefaultPos = { 0,0,0 };

	//顔料色
	DirectX::XMFLOAT4 mPigmentColor = { 1,1,1,1 };
	HSV mHSVColor = {0,1,1};
	ContainerData mContainerData = {};

	float mPrice = 100.f;
	bool isMovable = true;


public:
	Ingredient();
	~Ingredient() override = default;

	void OnStateDrag(float dt) override;
	void OnStateReleased(float dt) override;
	void Draw() override;

	/// @brief Reset Position to the defaultPos
	void ResetPos();
	void SetDefaultPos(DirectX::XMFLOAT3 pos);

	const DirectX::XMFLOAT4& GetColor();

	void SetPigmentColor(DirectX::XMFLOAT4 color);
	void SetHSVColor(float hue, float saturation, float value);

	void AddColor(float dt);
	void UpdateContainerColor();

	void Update(float dt) override;

	json SaveData() override;
	void LoadSaveData(json data) override;

	bool GetControllable()const { return isControllable; };
	void SetControllable(bool isControllable) { this->isControllable = isControllable; };

	void SetIngredientAlpha(float alpha);
	void SetMovable(bool isMovable) { this->isMovable = isMovable; };

	/// @brief 
	void ChargeCapacity();
	void SetCapacity(float _capacity);
	float GetCapacity()const { return mCapacity; };

	/// @brief キャパシティの比率を取得
	/// @return //(フルキャパは90)
	float GetCapacityPercentage() const { return mCapacity / 90.f; };

	void MixColorFunction(float dt);
};


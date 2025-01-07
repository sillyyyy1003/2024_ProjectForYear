#pragma once
#include "InteractiveMovableObject.h"

class WaterJug :public InteractiveMovableObject
{
private:
	//Default状態の時の位置
	DirectX::XMFLOAT3 mDefaultPos = { 0,0,0 };

	float mAccumulateTime = 0.0f;

public:
	WaterJug();
	~WaterJug() override = default;

	void OnStateDrag(float dt) override;
	void OnStateReleased(float dt) override;


	/// @brief Reset Position to the defaultPos
	void ResetPos();
	void SetDefaultPos(DirectX::XMFLOAT3 pos);


	void Update(float dt) override;

	json SaveData() override;
	void LoadSaveData(json data) override;

	bool GetControllable()const { return isControllable; };
	void SetControllable(bool isControllable) { this->isControllable = isControllable; };

	void AddWater(float dt);
};


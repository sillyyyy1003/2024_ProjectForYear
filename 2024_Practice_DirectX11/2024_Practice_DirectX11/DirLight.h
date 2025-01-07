#pragma once
#include "LightBase.h"
#include "SceneBase.h"

/// @brief 方向光
class DirLight :public LightBase
{

public:
	DirLight() = default;
	~DirLight() override = default;

	void Update(float dt) override;

	void LoadSaveData(json data, const char* objName);
	json SaveData();
};
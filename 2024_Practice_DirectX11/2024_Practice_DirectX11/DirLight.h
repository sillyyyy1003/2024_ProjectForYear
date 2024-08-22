#pragma once
#include "LightBase.h"

/// @brief •ûŒüŒõ
class DirLight :public LightBase
{
public:
	DirLight() = default;
	~DirLight() override = default;

	void Update(float dt) override;

};
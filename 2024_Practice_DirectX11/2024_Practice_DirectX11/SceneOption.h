#pragma once
#include "D2D_UIRect.h"
#include "D2D_UIStackContainer.h"
#include "ParticleEffect.h"
#include "PBRModel.h"
#include "SceneBase.h"
#include "UIButton.h"


class SceneOption : public SceneBase
{

	std::unique_ptr <ParticleEffect> testParticle;
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


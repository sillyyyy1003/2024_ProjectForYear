#pragma once
#include "D2D_UIRect.h"
#include "D2D_UIStackContainer.h"
#include "FireEffect.h"
#include "ParticleEffect.h"
#include "PBRModel.h"
#include "SceneBase.h"
#include "UIButton.h"


class SceneOption : public SceneBase
{

	//std::unique_ptr <ParticleEffect> testParticle;
	//std::unique_ptr<FireEffect> testParticle;
	std::unique_ptr<D2D_UIStackContainer> test;
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


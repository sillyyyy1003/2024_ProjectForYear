#pragma once
#include "SceneBase.h"
#include "UIStackContainer.h"

/// @brief This Scene is for info and operation of the mission
class SceneMission : public SceneBase
{
	std::unique_ptr<UIStackContainer> testContainer;
	
public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


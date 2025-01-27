#pragma once
#include "D2D_UIRect.h"
#include "SceneBase.h"
#include "StaticObject.h"
#include "UIButton.h"
#include "UISquare.h"
#include "UIStackContainer.h"
#include "UI_Button.h"

/// @brief This Scene is for info and operation of the mission
class SceneMission : public SceneBase
{
	std::unique_ptr<StaticObject> mWall;
	std::unique_ptr<StaticObject> mPaper;

	std::unique_ptr<UISquare> mBackground;
	std::unique_ptr<UISquare> mPaperBackground;
	std::unique_ptr<UISquare> mMissionOwner;

	std::unique_ptr<D2D_UIStackContainer> mColorSample;

	std::unique_ptr<D2D_UIStackContainer> MissionName;
	std::unique_ptr<D2D_UIStackContainer> MissionContent;
	std::unique_ptr<D2D_UIStackContainer> MissionOwnerName;
	std::unique_ptr<D2D_UIStackContainer> MissionOwnerNameContent;
	std::unique_ptr<D2D_UIStackContainer> MissionReward;
	std::unique_ptr<D2D_UIStackContainer> MissionRewardContent;

	std::unique_ptr<UIButton> mAccept;
	std::unique_ptr<UIButton> mBack;


	float floatingAmplitude = 5.f;
	float accumulateTime = 0.f;
	float pos = 0.0;

	SceneConfig::SceneIndex mNextScene = SceneConfig::SceneIndex::SCENE_NONE;


public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();

protected:
	void TriggerListener();
};


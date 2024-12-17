#pragma once
#include <cstdint>
#include "D2D_UIStackContainer.h"
#include "UIButton.h"
#include "UISquare.h"
#include "UIStackContainer.h"
#include "UI_Square.h"

namespace Tutorial
{
	enum TutorialPhase
	{
		Tutorial_01_Intro = 1 << 0,
		Tutorial_02_Workshop = 1 << 1,
		Tutorial_03_BuyMaterial = 1 << 2,
		Tutorial_04_FirstCraft = 1 << 3,
		Tutorial_05_ActionIntro = 1 << 4,
		Tutorial_06_ResetIntro = 1 << 5,
		Tutorial_07_Supplement = 1 << 6,
		Tutorial_08_SellIntro = 1 << 7,
		Tutorial_09_RateIntro = 1 << 8,
		Tutorial_10_LevelUpIntro = 1 << 9,

		Tutorial_00_None = 0,
	};
}

/// @brief チュートリアルの進捗を管理する
class TutorialManager
{
	//いまのチュートリアル状態を管理する
	uint32_t mTutorialPhaseState = Tutorial::Tutorial_01_Intro;
	//現在の状態は完成したか？
	bool mCurrentPhaseCompleted = false;

	//チュートリアルの時　シーン操作可能？
	bool isSceneFreeze = false;

	TutorialManager();
	~TutorialManager() = default;

	//Resource
	//First phase
	std::unique_ptr<UISquare> mFirstPhaseBackground;
	std::unique_ptr<UISquare> mFirstPhasePaper;
	std::unique_ptr<D2D_UIStackContainer> mFirstPhaseText;
	std::unique_ptr<D2D_UIStackContainer> mFirstPhaseSkipButton;
	std::unique_ptr<UISquare> mFirstPhaseFrontFade;
	float mFadeParam = 1.0f;
	float mAccumulateTime = 0.f;
	float mAmplitude = 10.0f;
	float posY = 0.0f;


public:

	bool isUseTutorial = true;

	//Get Instance
	static TutorialManager* Get()
	{
		static TutorialManager instance; return &instance;
	}

	void Init(json sceneData);
	void Update(float dt);
	void Draw();

	void CompleteCurrentPhase();
	void MoveToNextPhase();

	bool GetSceneFreeze() { return isSceneFreeze; };
	void SetSceneFreeze() { isSceneFreeze = true; };


	void DrawIntroPhase();

	json UnInit();



};

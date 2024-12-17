#pragma once
#include "SceneBase.h"
#include "UIStackContainer.h"
#include "UI_Button.h"

/// @brief This Scene is for info and operation of the mission
class SceneMission : public SceneBase
{
	std::unique_ptr<UIStackContainer> mPaperContainer;
	std::unique_ptr<UIStackContainer> mMissionOwner;
	std::vector<std::unique_ptr<UIStackContainer>> mIngredients;

	std::unique_ptr<UI_Button> mAccept;
	std::unique_ptr<UI_Button> mBack;

	std::unique_ptr<UIStackContainer> mColorSample;
	std::unique_ptr<UIStackContainer> mPSWord;

	
public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();
};


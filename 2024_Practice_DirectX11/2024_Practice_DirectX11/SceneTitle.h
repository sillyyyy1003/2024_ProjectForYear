#pragma once
#include "CandleLight.h"
#include "Potion.h"
#include "SceneBase.h"
#include "UI_Font.h"
#include "UI_Button.h"

//�^�C�g���V�[��
class SceneTitle:public SceneBase
{
private:
	bool isEditable = false;

	//======================================
	//	�V�[�����Ŏg���Ă���I�u�W�F�N�g���X�g
	//======================================

	std::shared_ptr<UI_Button> uiStart;		//Start Button
	std::unique_ptr<UI_Button> uiOption;
	std::unique_ptr<UI_Button>  uiExit;
	std::unique_ptr<UIStackContainer> uiTitle;	//Title





	std::unique_ptr<Potion> mWater;
	std::unique_ptr<CandleLight> mCandleLight;

public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();

	void TriggerListener();

	/// @brief Objects Update
	/// @param dt delta time
	void ObjectUpdate(float dt);

};


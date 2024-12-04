#pragma once
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

	std::shared_ptr<UI_Square> uiBg;		//Title Background
	std::shared_ptr<UI_Button> uiStart;		//Start Button
	std::unique_ptr<UI_Button> uiOption;
	std::unique_ptr<UI_Button>  uiExit;


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


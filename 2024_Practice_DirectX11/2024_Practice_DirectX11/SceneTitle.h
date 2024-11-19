#pragma once
#include "SceneBase.h"
#include "UIButton.h"
#include "UIFont.h"

//�^�C�g���V�[��
class SceneTitle:public SceneBase
{
private:


	std::unordered_map<std::string, std::vector<std::shared_ptr<CanvasUI>>> objManager;
	std::vector<std::string> mUiOrder = {};

	bool isEditable = false;

	//======================================
	//	�V�[�����Ŏg���Ă���I�u�W�F�N�g���X�g
	//======================================

	std::shared_ptr<CanvasUI> uiBg;		//Title Background
	std::shared_ptr<UIButton> uiStart;	//Start Button
	std::shared_ptr<UIButton> uiOption;	//Option Button
	std::shared_ptr<UIButton> uiExit;	//Exit Button



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


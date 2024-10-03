#pragma once
#include "SceneBase.h"
#include "UI_Button.h"

//�^�C�g���V�[��
class SceneTitle:public SceneBase
{
private:

	int mSceneIndex = 0;

	std::unordered_map<std::string, std::vector<CanvasUI*>> uiManager = {};
	std::vector<std::string> mUiOrder = {};

	bool isEditable = false;

public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();

private:
	/// @brief �V�[���̐؂�ւ�
	void ChangeScene();

};


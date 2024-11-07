#pragma once
#include "SceneBase.h"
#include "UIButton.h"
#include "UIFont.h"

//�^�C�g���V�[��
class SceneTitle:public SceneBase
{
private:

	int mSceneIndex = 0;

	std::unordered_map<std::string, std::vector<std::shared_ptr<CanvasUI>>> uiManager = {};
	std::vector<std::string> mUiOrder = {};

	std::unique_ptr<UIFont> testFont;

	bool isEditable = false;

public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void TriggerListener();

	/// @brief Objects Update
	/// @param dt delta time
	void ObjectUpdate(float dt);

	void Draw();

private:
	/// @brief �V�[���̐؂�ւ�
	void ChangeScene();

};


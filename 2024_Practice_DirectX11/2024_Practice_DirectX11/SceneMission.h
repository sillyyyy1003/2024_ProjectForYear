#pragma once
#include "SceneBase.h"
#include "UIStackContainer.h"

/// @brief This Scene is for info and operation of the mission
class SceneMission : public SceneBase
{
	std::unique_ptr<UIStackContainer> testContainer;
	
public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();
};


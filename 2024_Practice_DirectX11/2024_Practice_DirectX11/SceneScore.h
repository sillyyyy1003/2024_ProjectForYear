#pragma once
#include "UIStackContainer.h"

class SceneScore
{
	std::unique_ptr<UIStackContainer> mScoreBillboard;

public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();
};


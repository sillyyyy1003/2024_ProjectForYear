#pragma once
#include "SceneBase.h"
#include "Water.h"

class ScenePotion :public SceneBase
{
	std::unique_ptr<Water> mWater;


public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();
};


#pragma once
#include "SceneBase.h"


/// @brief SubScene of SceneGame For the LabScene/Mission Scene/Potion Scene
class SceneInGameOption:public SceneBase
{



public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();



};

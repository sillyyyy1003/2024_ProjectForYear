#pragma once
#include "SceneBase.h"

//�^�C�g���V�[��
class SceneTitle:public SceneBase
{
private:
	

public:
	/// @brief �f�[�^�̃��[�h�Ə�����
	void Init();

	/// @brief �f�[�^�̃Z�[�u���s��
	void UnInit();

	void Update(float dt);

	void Draw();

};


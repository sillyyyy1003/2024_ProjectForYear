#pragma once
#include "SceneBase.h"

/// @brief ���ׂẴV�[���̊Ǘ�
class SceneManager :public SceneBase
{
private:

	int mIndex = 0;//Scene Index 

public:
	/// @brief ���ʃI�u�W�F�N�g�̓ǂݍ��݁E�쐬
	void Init();

	/// @brief �f�[�^�ۑ��E���\�[�X�����[�X
	void UnInit();

	/// @brief 
	/// @param dt deltaTime
	void Update(float dt);

	/// @brief �`��
	void Draw();

private:
	/// @brief �V�[���̐؂�ւ�
	void ChangeScene();

};


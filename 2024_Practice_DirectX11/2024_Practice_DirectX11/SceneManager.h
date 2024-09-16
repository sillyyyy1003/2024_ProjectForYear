#pragma once
#include <unordered_map>

#include "SceneBase.h"

/// @brief ���ׂẴV�[���̊Ǘ�
class SceneManager :public SceneBase
{
private:

	int mIndex = 0;//Scene Index
	std::unordered_map<std::string, int> mSceneMap;

private:
	SceneManager(){};
	~SceneManager() = default;


public:
	static SceneManager* Get() { static SceneManager instance; return &instance; };

	/// @brief ���ʃI�u�W�F�N�g�̓ǂݍ��݁E�쐬
	void Init();

	/// @brief �f�[�^�ۑ��E���\�[�X�����[�X
	void UnInit();

	/// @brief 
	/// @param dt deltaTime
	void Update(float dt);

	/// @brief �`��
	void Draw();

	void SetScene(std::string sceneName);

private:
	/// @brief �V�[���̐؂�ւ�
	void ChangeScene();


};


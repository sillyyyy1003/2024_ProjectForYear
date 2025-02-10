#pragma once
#include "D2D_UIStackContainer.h"
#include "SceneBase.h"

class Player;

//�v���C���[�̃R�C������\������
class UIGoldDisplay :public Component
{
	//CoinIcon�\��

	//�����\��
	std::unique_ptr<D2D_UIStackContainer> mGoldUI = nullptr;
	//�v���C���[
	Player* mPlayer = nullptr;

	int mState = 0;	//UI�̏��
	float mAccumulateTime = 0.0f;
	float mDuration = 0.0f;

	int mStartNum = 0;
	int mIncreaseNum = 0;	//���₵������
	int mIncreaseResult = 0;

public:
	UIGoldDisplay() {};
	~UIGoldDisplay() override = default;


	void Init(Player* player,D2DUIConfig::FontSize fontSize);

	void Update(float dt);

	void Draw();

	/// @brief UI�̃t�H�[�}�b�g�ݒ�f�[�^�ǂݍ���
	/// @param data 
	void LoadSceneData(const json& data);

	//UI�̃t�H�[�}�b�g�ݒ�f�[�^�����o��
	json SaveData();

	void SetIncreaseFunction(float duration, int increaseNum);
protected:

	void OnStateNone(float dt);
	void OnStateCoinNumChange(float dt);
	
};


#pragma once
#include "InteractiveStaticObject.h"
#include "MovableStaticObject.h"

class NonDiegeticUI_PotTop :public InteractiveStaticObject
{
private:
	DirectX::XMFLOAT3 mDefaultPosition = {};
	DirectX::XMFLOAT3 mDefaultRotation = {};

	bool isFinishPotion = false;//�|�[�V�������I���E����֎~�E���o��

	float mAccumulateTime = 0.0f;	//�p������
	float mDuration = 0.f;			//���o����

	// Spin Parameter
	DirectX::XMFLOAT3 mStartPos = {};		//�ړ��̏����ʒu
	DirectX::XMFLOAT3 mStartRotation = {};	//��]�̏����p�x
	DirectX::XMFLOAT3 mRotateAmount = {};	//��]��
	DirectX::XMFLOAT3 mSpinCenter = {};		//��]���S
	float mSpinDegree = 0.0f;				//��]�^����
	int mAxis = 0;

	// Rise/Fall Parameter
	DirectX::XMFLOAT3 mDistance = {};

	DirectX::XMFLOAT2 mShakingAmplitude = {};	//�h�炷��
	DirectX::XMFLOAT2 mShakingDegree = {};		//�h�炷�p�x

	int mResultState = 0;
	int mNextResultState = 0;

	bool isWhiteOut = false;

public:
	NonDiegeticUI_PotTop();
	~NonDiegeticUI_PotTop() override = default;

	void PreUpdate(float dt) override;
	void GameUpdate(float dt) override;

	void LoadSaveData(json data) override;

	void OnStateClicked() override;
	void OnStateHover() override;
	void OnStateNone() override;
	void OnStateResult(float dt);

	void SetSpinIn(float spinDegree, const DirectX::XMFLOAT3& spinCenter, const DirectX::XMFLOAT3& startPos, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& startRotation, float duration, ObjectMovement::Axis);


	void OnStateRise(float dt);

	void OnStateSpin(float dt);

	void OnStateFall(float dt);

	void OnStateShaking(float dt);
	/// @brief �����ʒu�Ɖ�]�ɖ߂�
	void ResetPosition();

};


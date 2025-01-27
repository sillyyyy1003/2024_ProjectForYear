#pragma once
#include "InteractiveStaticObject.h"
#include "MovableStaticObject.h"

class NonDiegeticUI_PotTop :public InteractiveStaticObject
{
private:
	DirectX::XMFLOAT3 mDefaultPosition = {};
	DirectX::XMFLOAT3 mDefaultRotation = {};

	bool isFinishPotion = false;//ƒ|[ƒVƒ‡ƒ“ì‚èI‚í‚èE‘€ì‹Ö~E‰‰o’†

	float mAccumulateTime = 0.0f;	//Œp‘±ŠÔ
	float mDuration = 0.f;			//‰‰oŠÔ

	// Spin Parameter
	DirectX::XMFLOAT3 mStartPos = {};		//ˆÚ“®‚Ì‰ŠúˆÊ’u
	DirectX::XMFLOAT3 mStartRotation = {};	//‰ñ“]‚Ì‰ŠúŠp“x
	DirectX::XMFLOAT3 mRotateAmount = {};	//‰ñ“]—Ê
	DirectX::XMFLOAT3 mSpinCenter = {};		//‰ñ“]’†S
	float mSpinDegree = 0.0f;				//‰ñ“]‰^“®—Ê
	int mAxis = 0;

	// Rise/Fall Parameter
	DirectX::XMFLOAT3 mDistance = {};

	DirectX::XMFLOAT2 mShakingAmplitude = {};	//—h‚ç‚·•
	DirectX::XMFLOAT2 mShakingDegree = {};		//—h‚ç‚·Šp“x

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
	/// @brief ‰ŠúˆÊ’u‚Æ‰ñ“]‚É–ß‚é
	void ResetPosition();

};


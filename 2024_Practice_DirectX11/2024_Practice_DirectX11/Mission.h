#pragma once
#include "CameraBase.h"

class Mission
{
protected:
	float mResult = 100;	//	���ʓ_��
	DirectX::XMFLOAT4 mMissionColor = { 1,1,1 ,1};	//TargetColor
	float mReward = 100;
	std::string mMissionGrade;
	std::string mMissionContents;
	std::string mMissionOwner;		//�C����(Name+Icon)

public:

	Mission();
	~Mission();

	void InitMission();
	void RegisterAsCheckMission();

	std::string GetMissionContents();

	void CompleteMission(DirectX::XMFLOAT4 targetColor);
	std::string GetMissionResultGrade(DirectX::XMFLOAT4 targetColor);

	void SetReward(float reward) { mReward = reward; };
	void SetMissionColor(DirectX::XMFLOAT4 color) { mMissionColor = color; };

	DirectX::XMFLOAT4 GetMissionColor()const { return mMissionColor; };

};


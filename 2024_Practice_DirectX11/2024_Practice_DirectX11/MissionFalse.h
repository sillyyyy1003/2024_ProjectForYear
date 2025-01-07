#pragma once
#include "CameraBase.h"


class MissionFalse
{
protected:
	float mResult = 100;									//	結果点数
	DirectX::XMFLOAT4 mMissionColor = { 1,1,1 ,1};	//TargetColor
	float mReward = 100;
	std::string mMissionGrade;
	std::string mMissionContents;
	std::string mMissionOwner;		//任務主(Name+Icon)
	std::string mMissionReview;

public:

	MissionFalse();
	~MissionFalse();

	void InitMission();
	void RegisterAsCheckMission();

	std::string GetMissionContents();

	void CompleteMission(DirectX::XMFLOAT4 targetColor);
	std::string GetMissionResultGrade(DirectX::XMFLOAT4 targetColor);

	void SetReward(float reward) { mReward = reward; };
	void SetMissionColor(DirectX::XMFLOAT4 color) { mMissionColor = color; };

	DirectX::XMFLOAT4 GetMissionColor()const { return mMissionColor; };

};


#pragma once
#include "Mission.h"

class MissionManager
{
private:

	MissionManager() {};
	~MissionManager() = default;
	Mission* mCurrentCheckMission = nullptr; //���`�F�b�N���̔C��
	Mission* mCurrentMission = nullptr;	//���������̔C��
	Mission mCurrentMissionInstance;


public:
	static  MissionManager* Get() { static  MissionManager instance; return &instance; };

	void SetCurrentMission(Mission* mission);
	void SetCurrentCheckMission(Mission* mission);

	void ClearUpCurrentMission();
	void ClearUpCurrentCheckMission();

	void CompleteCurrentMission();
	void RegisterCheckMissionAsCurrentMission();

	Mission* GetCurrentMission();
	Mission* GetCurrentCheckMission() { return mCurrentCheckMission; };

	const Mission& GetCurrentMissionInstance() { return mCurrentMissionInstance; };
};


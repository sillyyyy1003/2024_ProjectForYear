#pragma once
#include "Mission.h"

class MissionManager
{
private:

	MissionManager() {};
	~MissionManager() = default;
	Mission* mCurrentCheckMission = nullptr; //今チェック中の任務
	Mission* mCurrentMission = nullptr;	//今完成中の任務
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


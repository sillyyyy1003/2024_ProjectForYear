#include "MissionManager.h"

void MissionManager::SetCurrentMission(Mission* mission)
{
	if (!mCurrentMission)
		mCurrentMission = mission;
}

void MissionManager::SetCurrentCheckMission(Mission* mission)
{
#ifdef _DEBUG
	if (!mission)
		DebugLog::LogError("Current Check Mission is NULL!!");
#endif
		if (!mCurrentCheckMission)
		mCurrentCheckMission = mission;
}

void MissionManager::ClearUpCurrentMission()
{
	mCurrentMission = nullptr;
}

void MissionManager::ClearUpCurrentCheckMission()
{
	mCurrentCheckMission = nullptr;
}

void MissionManager::CompleteCurrentMission()
{
	
}

void MissionManager::RegisterCheckMissionAsCurrentMission()
{
	if (mCurrentCheckMission)
	{
		//todo:transform instance to pointer
		mCurrentMissionInstance.SetMissionColor(mCurrentCheckMission->GetMissionColor());
		mCurrentMissionInstance.SetReward(200);
	}

}

Mission* MissionManager::GetCurrentMission()
{
	if (!mCurrentMission)
		DebugLog::LogWarning("GetNoCurrentMission");
	return mCurrentMission;
}

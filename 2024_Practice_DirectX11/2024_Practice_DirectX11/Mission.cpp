#include "Mission.h"

#include "MissionManager.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


Mission::Mission()
{
}

Mission::~Mission()
{
}

void Mission::InitMission()
{
}

void Mission::RegisterAsCheckMission()
{
	MissionManager::Get()->SetCurrentCheckMission(this);
}

std::string Mission::GetMissionContents()
{
	return mMissionContents;
}


void Mission::CompleteMission(DirectX::XMFLOAT4 targetColor)
{
	float deltaR, deltaG, deltaB;
	deltaR = targetColor.x - mMissionColor.x;
	deltaG = targetColor.y - mMissionColor.y;
	deltaB = targetColor.y - mMissionColor.y;

	float score = (float)std::sqrt(pow(deltaR, 2) + pow(deltaG, 2) + pow(deltaB, 2));
}

std::string Mission::GetMissionResultGrade(DirectX::XMFLOAT4 targetColor)
{
	float deltaR, deltaG, deltaB;
	deltaR = targetColor.x - mMissionColor.x;
	deltaG = targetColor.y - mMissionColor.y;
	deltaB = targetColor.y - mMissionColor.y;

	float deltaE =(float)std::sqrt(pow(deltaR, 2) + pow(deltaG, 2) + pow(deltaB, 2));
	if (deltaE <= 0.05f) return "S";      
	if (deltaE <= 0.15f) return "A";     
	if (deltaE <= 0.30f) return "B";   
	return "C";                           
}

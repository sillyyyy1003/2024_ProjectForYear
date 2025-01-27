#pragma once
#include "InteractiveStaticObject.h"

namespace Mission
{
	enum MissionState
	{
		STATE_NONE = 0,
		STATE_IMPLEMENT = 1,
		STATE_COMPLETE = 2, 

	};
	struct MissionData
	{
		std::string MissionName;
		std::string MissionContent;
		std::string MissionOwner;
		DirectX::XMFLOAT4 MissionColor;
		float MissionReward;
		int MissionState = STATE_NONE;
		int RemainDays = 0;
	};


	using MissionSet = std::vector<MissionData>;	//The Size of MissionSet is 3
	using MissionList = std::vector<MissionData>;	//Save all the missions

}

class MissionManager
{
private:

	MissionManager() {};
	~MissionManager() = default;
	Mission::MissionList mMissionList;

	Mission::MissionSet mCurrentMissionSet;							//現在表示されたMissionSet
	std::vector<InteractiveStaticObject*> mCurrentLevelMissionPaper;//Missionを描画する為の
	int mCurrentMissionIndex = -1;
	int mPreviousMissionIndex = -1;

	std::unique_ptr<InteractiveStaticObject> missionPaper1;
	std::unique_ptr<InteractiveStaticObject> missionPaper2;
	std::unique_ptr<InteractiveStaticObject> missionPaper3;


	//Mission 生成用データ
	DirectX::XMFLOAT4 mMissionColor = { 1,1,1,1 };
	HSV missionColor;
	char mMissionName[64];
	char mMissionContent[64];
	char mMissionOwnerName[64];
	float mMissionReward = 0.f;

	json MissionDataBuffer;//生成したデータの読み込み・書き出し用



public:
	static MissionManager* Get() { static  MissionManager instance; return &instance; };

	void Init();

	void InitLabMissionPaper();

	json SaveData();

	/// @brief Load MissionFalse from Json File
	/// @param sceneData Json File
	void LoadMissionList(json sceneData);

	/// @brief Missionを生成する用
	void MissionGenerator();

	//LabでのMission　Paperを描画する
	void DrawCurrentMissionSet();
	void UpdateCurrentMissionSet(float dt);


	Mission::MissionData GetCurrentMission();
	bool MissionPaperGetClicked();

	/// @brief 新しいミッションをチェックしているかどうか？
	/// @return 
	bool HasNewCurrentMission();

	/// @brief 進行中のミッションあるかどうか
	/// @return 
	bool HasCurrentMission();

	/// @brief 進行中のミッションを完成する
	void CompleteCurrentMission();
	
	
};


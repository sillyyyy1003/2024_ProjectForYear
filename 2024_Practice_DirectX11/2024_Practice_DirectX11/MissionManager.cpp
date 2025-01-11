#include "MissionManager.h"

#include "GameApp.h"
#include "RenderState.h"

void MissionManager::Init()
{
	InitLabMissionPaper();
}

void MissionManager::InitLabMissionPaper()
{
	/*mCurrentLevelMissionPaper.resize(3);*/
	missionPaper1 = std::make_unique<InteractiveStaticObject>();
	missionPaper2 = std::make_unique<InteractiveStaticObject>();
	missionPaper3 = std::make_unique<InteractiveStaticObject>();

	missionPaper1->Init(PrimitiveConfig::SQUARE, SceneManager::Get()->GetObj<Texture>("paper2"), "Paper1");
	missionPaper2->Init(PrimitiveConfig::SQUARE, SceneManager::Get()->GetObj<Texture>("paper2"), "Paper2");
	missionPaper3->Init(PrimitiveConfig::SQUARE, SceneManager::Get()->GetObj<Texture>("paper2"), "Paper3");

	missionPaper1->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_Primitives"),SceneManager::Get()->GetObj<PixelShader>("PS_InteractiveObjectNormal"));
	missionPaper2->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_Primitives"), SceneManager::Get()->GetObj<PixelShader>("PS_InteractiveObjectNormal"));
	missionPaper3->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_Primitives"), SceneManager::Get()->GetObj<PixelShader>("PS_InteractiveObjectNormal"));

	mCurrentLevelMissionPaper.push_back(missionPaper1.get());
	mCurrentLevelMissionPaper.push_back(missionPaper2.get());
	mCurrentLevelMissionPaper.push_back(missionPaper3.get());

}

json MissionManager::SaveData()
{
	json Data;
	Data["Mission"]= MissionDataBuffer;
	Data["PaperData"]["Paper1"] = missionPaper1->SaveData();
	Data["PaperData"]["Paper2"] = missionPaper2->SaveData();
	Data["PaperData"]["Paper3"] = missionPaper3->SaveData();
	return Data;
}

void MissionManager::LoadMissionList(json sceneData)
{
	MissionDataBuffer = sceneData["Mission"];
	mCurrentMissionSet.resize(3);
	int index = 0;
	for(const auto& mission: MissionDataBuffer)
	{
		mCurrentMissionSet[index].MissionColor = { mission["MissionColor"][0].get<float>(),
				mission["MissionColor"][1].get<float>(),
				mission["MissionColor"][2].get<float>(),
				mission["MissionColor"][3].get<float>() };

		mCurrentMissionSet[index].MissionName = mission["MissionName"].get<std::string>();
		mCurrentMissionSet[index].MissionContent = mission["MissionContent"].get<std::string>();
		mCurrentMissionSet[index].MissionReward = mission["MissionReward"].get<float>();
		mCurrentMissionSet[index].MissionOwner = mission["MissionOwner"].get<std::string>();
		index++;
	}

	missionPaper1->LoadSaveData(sceneData["PaperData"],"Paper1");
	missionPaper2->LoadSaveData(sceneData["PaperData"], "Paper2");
	missionPaper3->LoadSaveData(sceneData["PaperData"], "Paper3");
}

void MissionManager::MissionGenerator()
{
	if(ImGui::Begin("MissionFalse Generator"))
	{
		ImGui::InputText("MissionFalse Name", mMissionName, IM_ARRAYSIZE(mMissionName));

		ImGui::InputText("MissionFalse Content", mMissionContent, IM_ARRAYSIZE(mMissionContent));

		ImGui::InputText("MissionFalse Owner", mMissionOwnerName, sizeof(mMissionOwnerName));

		float hsv[3] = { missionColor.hue,missionColor.saturation,missionColor.value };
		ImGui::InputFloat3("HSV", hsv);
		missionColor = { hsv[0],hsv[1],hsv[2] };
		mMissionColor = HSVtoRGB(missionColor);
		float colorpicker[4] = { mMissionColor.x,mMissionColor.y,mMissionColor.z,mMissionColor.w };
		ImGui::ColorPicker4("Color", colorpicker);

		ImGui::InputFloat("Reward", &mMissionReward);

		if(ImGui::Button("InsertData"))
		{
			json data = {
				{"MissionName",mMissionName},
				{"MissionContent",mMissionContent},
				{"MissionOwner",mMissionOwnerName},
				{"MissionColor",{mMissionColor.x,mMissionColor.y,mMissionColor.z,mMissionColor.w}},
				{"MissionReward",mMissionReward}
			};
		  MissionDataBuffer.push_back(data);
		}

		ImGui::Separator();
		ImGui::Text("Existing Missions:");
		for (const auto& mission : MissionDataBuffer)
		{
			ImGui::BulletText("Name: %s, Content: %s, Owner: %s, Reward: %d,Color:%lf,%lf,%lf,%lf",
				mission["MissionName"].get<std::string>().c_str(),
				mission["MissionContent"].get<std::string>().c_str(),
				mission["MissionOwner"].get<std::string>().c_str(),
				mission["MissionReward"].get<int>(),
				mission["MissionColor"][0].get<float>(),
				mission["MissionColor"][1].get<float>(),
				mission["MissionColor"][2].get<float>(),
				mission["MissionColor"][3].get<float>());
		}

	}
	ImGui::End();
}

void MissionManager::DrawCurrentMissionSet()
{
	GameApp::SetBlendState(RenderState::BSTransparent);
	for (int i = 0; i < mCurrentMissionSet.size(); i++)
	{
		if (mCurrentMissionSet[i].MissionState != Mission::STATE_COMPLETE)
		{
			mCurrentLevelMissionPaper[i]->Draw();
		}
	}
	GameApp::SetBlendState(nullptr);
}

void MissionManager::UpdateCurrentMissionSet(float dt)
{
	for (int i = 0; i < mCurrentMissionSet.size(); i++)
	{
		if (mCurrentMissionSet[i].MissionState != Mission::STATE_COMPLETE)
		{
			mCurrentLevelMissionPaper[i]->Update(dt);
		}
	}
}

Mission::MissionData MissionManager::GetCurrentMission()
{
	return mCurrentMissionSet[mCurrentMissionIndex];
}

bool MissionManager::MissionPaperGetClicked()
{
	for (int i = 0; i < mCurrentMissionSet.size(); i++)
	{
		if(mCurrentLevelMissionPaper[i]->GetClicked())
		{
			mCurrentMissionIndex = i;
			return true;
		}
	}

	return false;
}

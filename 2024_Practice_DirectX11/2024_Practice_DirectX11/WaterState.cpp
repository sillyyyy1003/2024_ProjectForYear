#include "WaterState.h"

void WaterState::Init()
{
	mWaterStates[WaterStateConfig::WaterState::STATE_STILL] = {
		{0,0,0},
		0,
		0,
		0,
		0,
	};

	mWaterStates[WaterStateConfig::WaterState::STATE_RIPPLING] = {
		{0,0,0},
		0,
		0,
		0,
		0,
		0,
	};

	mWaterStates[WaterStateConfig::WaterState::STATE_BOILING] = {
		{0,0,0},
		0,
		0,
		0,
		0,
	};

	mWaterStates[WaterStateConfig::WaterState::STATE_VIOLENT] = {
		{0,0,0},
		0,
		0,
		0,
		0,
	};

}

void WaterState::LoadWaterStateData(json data)
{
	for (int i = 0; i < static_cast<unsigned int>(WaterStateConfig::WaterState::STATE_MAX); i++)
	{
		WaterStateConfig::WaterState state = static_cast<WaterStateConfig::WaterState>(i);
		const char* objName = WaterStateConfig::DefaultData[state].c_str();
		mWaterStates[state].center = { data[objName]["Center"][0],data[objName]["Center"][1],data[objName]["Center"][2] };
		mWaterStates[state].maxAmplitude = data[objName]["MaxAmplitude"][0];
		mWaterStates[state].minAmplitude = data[objName]["MinAmplitude"][0];
		mWaterStates[state].frequency = data[objName]["Frequency"][0];
		mWaterStates[state].speed = data[objName]["Speed"][0];
		mWaterStates[state].sigma = data[objName]["Sigma"][0];
	}
}

void WaterState::SetCurrentWaterParam(WaterStateConfig::WaterState state)
{
	currentStateIndex = static_cast<int>(state);
}

json WaterState::SaveWaterParam()
{
	json data;
	for (int i = 0; i < static_cast<unsigned int>(WaterStateConfig::WaterState::STATE_MAX); i++)
	{
		WaterStateConfig::WaterState state = static_cast<WaterStateConfig::WaterState>(i);
		const char* objName = objName = WaterStateConfig::DefaultData[state].c_str();
		data[objName]["Center"] = { mWaterStates[state].center.x,mWaterStates[state].center.y,mWaterStates[state].center.z };
		data[objName]["MaxAmplitude"] = { mWaterStates[state].maxAmplitude };
		data[objName]["MinAmplitude"] = { mWaterStates[state].minAmplitude };
		data[objName]["Frequency"] = { mWaterStates[state].frequency };
		data[objName]["Speed"] = { mWaterStates[state].speed };
		data[objName]["Sigma"] = { mWaterStates[state].sigma };

	}
	return data;
}



const WaterStateConfig::WaterStateParam& WaterState::GetCurrentWaterParam()
{
	return mWaterStates[static_cast<WaterStateConfig::WaterState>(currentStateIndex)];
}

void WaterState::DebugFunction()
{
	// 状态名称列表
	const char* stateNames[] = { "Still", "Rippling", "Boiling", "Violent" };
	int currentState = currentStateIndex;
	if(ImGui::Begin("WaterParamOption"))
	{
		// 下拉菜单选择状态
		if (ImGui::Combo("Select Water State", &currentState, stateNames, IM_ARRAYSIZE(stateNames)))
		{
			currentStateIndex = currentState;
		}

		WaterStateConfig::WaterState curState = static_cast<WaterStateConfig::WaterState>(currentStateIndex);
		float centerPos[3] = { mWaterStates[curState].center.x,mWaterStates[curState].center.y,mWaterStates[curState].center.z };
		ImGui::InputFloat3("Center", centerPos);
		mWaterStates[curState].center = { centerPos[0],centerPos[1],centerPos[2]};

		float maxAmplitude = mWaterStates[curState].maxAmplitude;
		ImGui::InputFloat("MaxAmplitude", &maxAmplitude);
		mWaterStates[curState].maxAmplitude = maxAmplitude;

		float amplitude = mWaterStates[curState].minAmplitude;
		ImGui::InputFloat("MinAmplitude", &amplitude);
		mWaterStates[curState].minAmplitude = amplitude;

		float frequency = mWaterStates[curState].frequency;
		ImGui::InputFloat("Frequency", &frequency);
		mWaterStates[curState].frequency = frequency;

		float speed = mWaterStates[curState].speed;
		ImGui::InputFloat("Speed", &speed);
		mWaterStates[curState].speed = speed;

		float sigma = mWaterStates[curState].sigma;
		ImGui::InputFloat("Sigma", &sigma);
		mWaterStates[curState].sigma = sigma;

	}
	ImGui::End();
}



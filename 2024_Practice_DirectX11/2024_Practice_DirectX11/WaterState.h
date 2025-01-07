#pragma once
#include "D3DUtil.h"
#include <map>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include "SceneBase.h"

namespace WaterStateConfig
{
	struct WaterStateParam
	{
		DirectX::XMFLOAT3  center = {}; // 波の中心位置
		float maxAmplitude = 0.f;			// 波の幅
		float minAmplitude = 0.f;
		float frequency = 0.f;			// 波の頻度
		float speed = 0.f;				// 波のスビード
		float sigma = 0.f;				// 波の拡散範囲
	};

	enum class WaterState:unsigned int
	{
		STATE_STILL,
		STATE_RIPPLING,
		STATE_BOILING,
		STATE_VIOLENT,

		STATE_MAX,
	};

	inline std::map<WaterState, std::string> DefaultData = {
		{ WaterState::STATE_STILL,"Still"},
		{ WaterState::STATE_RIPPLING,"Rippling"},
		{ WaterState::STATE_BOILING,"Boiling"},
		{ WaterState::STATE_VIOLENT,"Violent"},
	};

	enum class WaterBoilingState :int
	{
		STATE_STILL = 0,	// Only move when trigger is true;
		STATE_BOILING = 1,	// Consistently boiling
#ifdef _DEBUG
		STATE_EXPERIMENT = 2, 
#endif
		STATE_MAX = 99,
	};

}

//水の沸騰状態を管理する
//todo: param would be changed considering the fire
class WaterState
{
	/// @brief
	///	@param string 保存された状態の名前
	///	@param WaterParam 波の状態
	//std::map<std::string, WaterStateConfig::WaterParam> mWaterState;
	std::map<WaterStateConfig::WaterState, WaterStateConfig::WaterStateParam> mWaterStates;

	//Debug
	int currentStateIndex = 0;
public:
	WaterState() {};
	~WaterState() = default;

	void Init();

	void LoadWaterStateData(json data);

	void SetCurrentWaterParam(WaterStateConfig::WaterState state);

	json SaveWaterParam();
	const WaterStateConfig::WaterStateParam& GetCurrentWaterParam();
	void DebugFunction();
};


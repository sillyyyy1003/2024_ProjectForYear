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
		DirectX::XMFLOAT3  center = {}; // �g�̒��S�ʒu
		float maxAmplitude = 0.f;			// �g�̕�
		float minAmplitude = 0.f;
		float frequency = 0.f;			// �g�̕p�x
		float speed = 0.f;				// �g�̃X�r�[�h
		float sigma = 0.f;				// �g�̊g�U�͈�
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

//���̕�����Ԃ��Ǘ�����
//todo: param would be changed considering the fire
class WaterState
{
	/// @brief
	///	@param string �ۑ����ꂽ��Ԃ̖��O
	///	@param WaterParam �g�̏��
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


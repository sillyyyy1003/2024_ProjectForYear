#pragma once
#include <map>
#include <memory>
#include <string>
#include "SceneBase.h"

namespace PlayerConfig
{
	enum PigmentColor :uint8_t
	{
		RED = 0,
		YELLOW = 1,
		BLUE = 2,
		COLOR_MAX
	};
}

class Player :public Component
{
private:
	/// @brief
	///	@param PigmentColor ColorName
	///	@param float Current Capacity
	std::unordered_map<PlayerConfig::PigmentColor, float> mCapacities;

	float mGold = 100;	//プレヤーが持つ金
	float mWitchLevel = 1;//ギルドレベル

	int mDay = 1;//Calculate the day

public:
	Player();
	~Player() = default;

	void LoadPlayerData(json data);
	json SaveData();

	const float& GetPlayerGold() const { return mGold; };
	std::string GetPlayerGold();

	std::string GetDayNum();

	const float& GetCapacities(PlayerConfig::PigmentColor color);
	void SaveCapacitiesData(PlayerConfig::PigmentColor color, float capacity);

	float GetWitchLevel() const { return mWitchLevel; };
	void LevelUp();
	void EndToday();

	void ChargeForIngredients();
};


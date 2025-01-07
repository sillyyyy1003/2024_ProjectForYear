#include "Player.h"

#include "IngredientManager.h"

Player::Player()
{
}

void Player::LoadPlayerData(json data)
{
	mGold = { data["Gold"][0]};
	mWitchLevel = { data["WitchLevel"][0]};
	
	mCapacities[PlayerConfig::PigmentColor::RED] = { data["Capacities"]["Red"][0]};
	mCapacities[PlayerConfig::PigmentColor::YELLOW] = { data["Capacities"]["Yellow"][0]};
	mCapacities[PlayerConfig::PigmentColor::BLUE] = { data["Capacities"]["Blue"][0]};
}

json Player::SaveData()
{
	json data;
	data["Gold"] = { mGold };
	data["WitchLevel"] = { mWitchLevel };
	data["Day"] = { mDay };
	data["Capacities"]["Red"] = {mCapacities[PlayerConfig::PigmentColor::RED]};
	data["Capacities"]["Yellow"] = { mCapacities[PlayerConfig::PigmentColor::RED] };
	data["Capacities"]["Blue"] = { mCapacities[PlayerConfig::PigmentColor::RED] };
	return data;
}

std::string Player::GetPlayerGold()
{
	std::string outputStr;
	outputStr = std::to_string((int)mGold);
	return outputStr;
}

std::string Player::GetDayNum()
{
	std::string outputStr;
	outputStr = "Day: " + std::to_string(mDay);
	return outputStr;
}

const float& Player::GetCapacities(PlayerConfig::PigmentColor color)
{
	return mCapacities[color];
}

void Player::SaveCapacitiesData(PlayerConfig::PigmentColor color, float capacity)
{
	mCapacities[color] = capacity;
}

void Player::LevelUp()
{
	mWitchLevel++;
}

void Player::EndToday()
{
	//Add day
	mDay++;

	//Charge all ingredient
	IngredientManager::Get()->ChargeAllIngredient();

}

void Player::ChargeForIngredients()
{
	mGold -= 100.f * mWitchLevel;
	if (mGold <= 0)mGold = 0;
	
}

#pragma once
#include <vector>
#include "Ingredient.h"

class IngredientManager
{
protected:

	float mIngredientLevelFactor = 0.2f;
	std::unordered_map<PlayerConfig::PigmentColor, Ingredient*>mCurrentIngredients;

	IngredientManager();
	~IngredientManager() = default;

	Ingredient* mCurrentIngredient = nullptr;

public:

	static IngredientManager* Get() { static IngredientManager instance; return &instance; };
	void Init();
	
	void Update(float dt);
	void Draw();
	
	/// @brief Init Player Capacity Data to current Ingredients
	void InitPotionCapacity();

	/// @brief Register ingredient data to manager
	/// @param ingredient 
	/// @param color Ingredient name
	void RegisterIngredient(Ingredient* ingredient, PlayerConfig::PigmentColor color);

	/// @brief 操作中のものをセットする
	/// @param ingredient 
	void SetControllableIngredient(Ingredient* ingredient);

	/// @brief Free all ingredient control status
	void ClearCurrentIngredient();

	void LockAllIngredient();

	/// @brief Charge all ingredient capacity to full
	void ChargeAllIngredient();

	void UpdateCapacityData();

	/// @brief Set Ingredient alpha according to player level
	void SetIngredientsAlpha();

	bool GetControllableIngredient();

};



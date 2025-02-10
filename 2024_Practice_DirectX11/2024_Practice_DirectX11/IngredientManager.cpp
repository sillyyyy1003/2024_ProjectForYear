#include "IngredientManager.h"

IngredientManager::IngredientManager()
{
	
}

void IngredientManager::Init()
{
	ClearCurrentIngredient();
}

void IngredientManager::InitPotionCapacity()
{
	Player* player = SceneManager::Get()->GetObj<Player>("player").get();
	for (int i = 0; i < PlayerConfig::COLOR_MAX; i++)
	{
		mCurrentIngredients[static_cast<PlayerConfig::PigmentColor>(i)]->SetCapacity(player->GetCapacities(static_cast<PlayerConfig::PigmentColor>(i)));
	}
}

void IngredientManager::Update(float dt)
{
	//Ingredient 状態管理
	if(mCurrentIngredient)
	{
		for(const auto& element:mCurrentIngredients)
		{
			if (element.second != mCurrentIngredient)
			{
				element.second->SetControllable(false);
			}
		}
	}

}

void IngredientManager::Draw()
{
	
}

void IngredientManager::RegisterIngredient(Ingredient* ingredient, PlayerConfig::PigmentColor color)
{
	mCurrentIngredients[color] = ingredient;
}

void IngredientManager::SetControllableIngredient(Ingredient* ingredient)
{
	if (!mCurrentIngredient)
		mCurrentIngredient = ingredient;
}

void IngredientManager::ClearCurrentIngredient()
{
	mCurrentIngredient = nullptr;
	for (const auto& element : mCurrentIngredients)
	{
		element.second->SetControllable(true);
	}
}

void IngredientManager::LockAllIngredient()
{
	for (const auto& element : mCurrentIngredients)
	{
		element.second->SetControllable(false);
	}
}

void IngredientManager::ChargeAllIngredient()
{
	//Charge
	for (const auto& element : mCurrentIngredients)
	{
		element.second->ChargeCapacity();
	}

	Player* player = SceneManager::Get()->GetObj<Player>("player").get();
	player->ChargeForIngredients();
	

}

//void IngredientManager::UpdateCapacityData()
//{
//	Player* player = SceneManager::Get()->GetObj<Player>("player").get();
//
//	for (int i = 0; i < PlayerConfig::COLOR_MAX; i++)
//	{
//		PlayerConfig::PigmentColor color = static_cast<PlayerConfig::PigmentColor>(i);
//		float capacity = mCurrentIngredients[color]->GetCapacity();
//		player->SaveCapacitiesData(color,capacity);
//	}
//}

void IngredientManager::SetIngredientsAlpha()
{
	Player* player = SceneManager::Get()->GetObj<Player>("player").get();
	for (const auto& element : mCurrentIngredients)
	{
		float alpha = mIngredientLevelFactor * player->GetWitchLevel();
		element.second->SetIngredientAlpha(alpha);
	}


}

bool IngredientManager::GetControllableIngredient()
{
	if (mCurrentIngredient != nullptr)
		return true;
	else
		return false; 
}


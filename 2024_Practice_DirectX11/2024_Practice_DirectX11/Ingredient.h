#pragma once
#include "Object.h"
#include "Primitive.h"
#include "UIButton.h"

/// Potionを構成する素材
class Ingredient :public Object
{
	float mBasicPrice = 100.0f;
	float mCurrPrice = 100.0f;

	float mFreshness = 100.0f;		//新鮮度
	float mBasicDecayRate = 0.01f;
	int	mIngredientState = 0;		//素材の状態

	float mShelfLife = 100.0f;		//保存時間


protected:

	std::string mIngredientName="default ingredient";

public:
	Ingredient();
	~Ingredient() = default;

	/// @brief Init Ingredient outline
	/// @param _kind 
	/// @param _fileName 
	void Init(PrimitiveKind _kind,const char* _fileName);

	/// @brief 金額を設定する
	void SetPrice(float _price);

	/// @brief 名前を設定する
	void SetName(const char* _ingredientName);


	void Update(float dt);
	void Draw();

	void UpdatePrice();

	/// @brief 素材の情報
	/// @return format: Name:xxx Freshness:xxx Price:xxx
	std::string GetIngredientInfo();

protected:

	void PreUpdate(float dt) override;
	void GameUpdate(float dt) override;
	void LateUpdate(float dt) override;


};
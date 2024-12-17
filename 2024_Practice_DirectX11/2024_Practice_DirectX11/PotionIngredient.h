#pragma once
#include "SceneBase.h"
#include "UI_MoveObject.h"

//todo:this is not is-a relationship
//todo:need to fix to a has-a relationship
class PotionIngredient :public UI_MoveObject
{
private:

	float mAccumulateTime = 0.0f;
	float mCapacity = 100.f;


	DirectX::XMFLOAT2 mDefaultPos = {};
	DirectX::XMFLOAT4 mIngredientColor = { 1,1,1,1 };

	float mAlpha = 0.2f;//颜料的比重

	std::unique_ptr<UI_Square> mFrontColor;

	
public:

	PotionIngredient() = default;
	~PotionIngredient() override = default;

	void Update(float dt) override;
	void PreUpdate(float dt) override;
	void GameUpdate(float dt) override;

	void LoadSaveData(json saveData) override;

	/// @brief Reset Position to the defaultPos
	void ResetPos();

	/// @brief SetPosition to the 
	/// @param pos 
	void SetDefaultPos(DirectX::XMFLOAT2 pos);

	const DirectX::XMFLOAT4& GetColor();;

	void OnStateHover(float dt) override;
	void OnStateDrag(float dt)override;
	void OnStateRelease(float dt) override;
	json SaveData()override;

	void AddColor(float dt);
	void UpdateFrontColor();
	
	void Draw() override;

};


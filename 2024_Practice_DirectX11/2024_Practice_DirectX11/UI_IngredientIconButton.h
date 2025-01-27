#pragma once
#include "Ingredient.h"
#include "UI_IconButton.h"
class UI_IngredientIconButton : public UI_IconButton
{
	Ingredient* mIngredient = nullptr;			//�F�X�V���鎞��
	std::unique_ptr<UI_Square>	mCapacity;		//�L���p�V�e�B��\������

public:
	void Init(const char* bgTex, const char* iconTex, const char* objName) override;

	void OnStateNone(float dt) override;
	void SetIngredient(Ingredient* ingredient) { mIngredient = ingredient; };
	void OnStateHover(float dt) override;
	void OnStatePress(float dt) override;
	void OnStateTrigger(float dt) override;

	void Draw() override;

	//void LoadSaveData(json data) override;
};


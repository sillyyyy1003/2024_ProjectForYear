#pragma once
#include "CanvasUI.h"
#include "Ingredient.h"
#include "SceneBase.h"
#include "Primitive.h"
#include "UI2D.h"
#include "UI_Button.h"
#include "Water.h"

/// @brief メインのラブシーン
///	Potion Making Scene
class SceneLab :public SceneBase
{
	int mSceneIndex = 0;

public:
	void Init();
	void UnInit();
	void Update(float dt);
	void Draw();

	void ChangeScene();

private:

	std::unique_ptr<Water> water = nullptr;
	std::unique_ptr<Primitive> table = nullptr;

	std::unique_ptr<UI2D> infoBox = nullptr;
	std::unique_ptr<UI2D> time = nullptr;

	std::unique_ptr<UI_Button> infoButton = nullptr;

	std::unique_ptr<Ingredient> testIngredient = nullptr;

};

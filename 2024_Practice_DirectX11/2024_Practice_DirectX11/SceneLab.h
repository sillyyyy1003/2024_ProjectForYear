#pragma once
#include "CanvasUI.h"
#include "SceneBase.h"
#include "Primitive.h"
#include "Water.h"

/// @brief メインのラブシーン
///	Potion Making Scene
class SceneLab :public SceneBase
{
public:
	void Init();
	void UnInit();
	void Update(float dt);
	void Draw();

private:

	std::unique_ptr<Primitive> circle = nullptr;
	
	std::unique_ptr<Water> water = nullptr;
	std::unique_ptr<Primitive> table = nullptr;

};

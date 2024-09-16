#pragma once
#include "CanvasUI.h"
#include "SceneBase.h"
#include "Primitive.h"

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

	std::unique_ptr<Primitive> water = nullptr;
	std::unique_ptr<Primitive> floor = nullptr;

};

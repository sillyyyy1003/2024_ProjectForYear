#pragma once
#include "Cylinder.h"
#include "PBRModel.h"
#include "SceneBase.h"
#include "Sphere.h"
#include "Square.h"
#include "UIStackContainer.h"
#include "UI_Square.h"

class SceneOption : public SceneBase
{

	//std::shared_ptr<UI_Square> test;
	bool isWrite = false;

	std::shared_ptr<Cylinder> cylinder;
	std::shared_ptr<Sphere> sphere;

	std::unique_ptr<PBRModel> model;
	std::unique_ptr<UIStackContainer> container;

	std::unique_ptr<Square> testOutline;


public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


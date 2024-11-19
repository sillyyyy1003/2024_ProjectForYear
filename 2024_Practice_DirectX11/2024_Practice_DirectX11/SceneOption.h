#pragma once
#include "Cylinder.h"
#include "SceneBase.h"
#include "Sphere.h"
#include "UI_Capsule.h"

class SceneOption : public SceneBase
{

	std::shared_ptr<UI_Primitive> test;
	bool isWrite = false;

	std::shared_ptr<Cylinder> cylinder;
	std::shared_ptr<Sphere> sphere;



public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();
};


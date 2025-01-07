#pragma once
#include "SceneBase.h"


/// @brief SubScene of SceneGame For the LabScene/Mission Scene/Potion Scene
class SceneInGameOption:public SceneBase
{



public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();



};


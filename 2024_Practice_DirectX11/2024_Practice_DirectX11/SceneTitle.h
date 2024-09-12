#pragma once
#include "SceneBase.h"

//タイトルシーン
class SceneTitle:public SceneBase
{
private:
	

public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();

};


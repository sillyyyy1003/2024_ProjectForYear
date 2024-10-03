#pragma once
#include "SceneBase.h"
#include "UI_Button.h"

//タイトルシーン
class SceneTitle:public SceneBase
{
private:

	int mSceneIndex = 0;

	std::unordered_map<std::string, std::vector<CanvasUI*>> uiManager = {};
	std::vector<std::string> mUiOrder = {};

	bool isEditable = false;

public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();

private:
	/// @brief シーンの切り替え
	void ChangeScene();

};


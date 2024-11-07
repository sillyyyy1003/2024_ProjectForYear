#pragma once
#include "SceneBase.h"
#include "UIButton.h"
#include "UIFont.h"

//タイトルシーン
class SceneTitle:public SceneBase
{
private:

	int mSceneIndex = 0;

	std::unordered_map<std::string, std::vector<std::shared_ptr<CanvasUI>>> uiManager = {};
	std::vector<std::string> mUiOrder = {};

	std::unique_ptr<UIFont> testFont;

	bool isEditable = false;

public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void TriggerListener();

	/// @brief Objects Update
	/// @param dt delta time
	void ObjectUpdate(float dt);

	void Draw();

private:
	/// @brief シーンの切り替え
	void ChangeScene();

};


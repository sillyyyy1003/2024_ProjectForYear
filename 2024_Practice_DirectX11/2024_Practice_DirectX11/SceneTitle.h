#pragma once
#include "SceneBase.h"
#include "UI_Font.h"
#include "UI_Button.h"

//タイトルシーン
class SceneTitle:public SceneBase
{
private:
	bool isEditable = false;

	//======================================
	//	シーン中で使われているオブジェクトリスト
	//======================================

	std::shared_ptr<UI_Square> uiBg;		//Title Background
	std::shared_ptr<UI_Button> uiStart;		//Start Button
	std::unique_ptr<UI_Button> uiOption;
	std::unique_ptr<UI_Button>  uiExit;


public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();

	void TriggerListener();

	/// @brief Objects Update
	/// @param dt delta time
	void ObjectUpdate(float dt);

};


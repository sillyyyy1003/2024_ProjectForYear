#pragma once
#include "SceneBase.h"
#include "UIButton.h"
#include "UIFont.h"
#include "UI_Button.h"

//タイトルシーン
class SceneTitle:public SceneBase
{
private:
	std::vector<std::string> mUiOrder = {};

	bool isEditable = false;

	//======================================
	//	シーン中で使われているオブジェクトリスト
	//======================================

	std::shared_ptr<UI_Square> uiBg;		//Title Background
	std::shared_ptr<UI_Button> uiStart;		//Start Button
	std::unique_ptr<UI_Button> uiOption;
	std::unique_ptr<UI_Button>  uiExit;

	//std::unique_ptr<UI_Primitive> uiBg;		//Title Background
	//std::unique_ptr<UIButton> uiStart;		//Start Button
	//std::unique_ptr<UIButton> uiOption;		//Option Button
	//std::unique_ptr<UIButton> uiExit;		//Exit Button


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


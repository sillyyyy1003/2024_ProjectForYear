#pragma once
#include "CandleLight.h"
#include "Potion.h"
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

	std::shared_ptr<UI_Button> uiStart;		//Start Button
	std::unique_ptr<UI_Button> uiOption;
	std::unique_ptr<UI_Button>  uiExit;
	std::unique_ptr<UIStackContainer> uiTitle;	//Title





	std::unique_ptr<Potion> mWater;
	std::unique_ptr<CandleLight> mCandleLight;

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


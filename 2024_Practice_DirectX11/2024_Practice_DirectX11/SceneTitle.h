#pragma once
#include "CandleLight.h"
#include "D2D_UIStackContainer.h"
#include "Potion.h"
#include "PotionEffect.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "StaticObject.h"
#include "UIButton.h"


//タイトルシーン
class SceneTitle:public SceneBase
{
private:
	bool isEditable = false;

	//======================================
	//	シーン中で使われているオブジェクトリスト
	//======================================


	std::unique_ptr<D2D_UIStackContainer> mTitle;
	std::unique_ptr<UIButton> mStart;
	std::unique_ptr<UIButton> mOption;
	std::unique_ptr<UIButton> mExit;


	std::unique_ptr<Potion> mWater;
	std::unique_ptr<CandleLight> mCandleLight;
	std::unique_ptr<StaticObject> mPot;


	std::unique_ptr<PotionEffect> mPotionEffect;

	SceneConfig::SceneIndex mNextScene = SceneConfig::SceneIndex::SCENE_NONE;

	//Texture List
	PBRConfig::PBRTexList pbrTexList;

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


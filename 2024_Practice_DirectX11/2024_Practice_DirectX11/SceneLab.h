#pragma once
#include "CandleLight.h"
#include "Cylinder.h"
#include "D2D_UIStackContainer.h"
#include "Ingredient.h"
#include "InteractiveStaticObject.h"
#include "MissionPaper.h"
#include "MovableStaticObject.h"
#include "PointLight.h"
#include "SceneBase.h"
#include "Sphere.h"
#include "StaticObject.h"
#include "UIGoldDisplay.h"
#include "UIStackContainer.h"

#include "Water.h"

class SceneLab :public SceneBase
{
	//Texture List
	PBRConfig::PBRTexList pbrTexList;
	
	//左の蝋燭に影響されるオブジェクト
	std::unordered_map<std::string, StaticObject*> leftStaticObjList;
	//右の蝋燭に影響されるオブジェクト
	std::unordered_map<std::string, StaticObject*> rightStaticObjList;

	std::unique_ptr<InteractiveStaticObject> mPot;
	std::unique_ptr<StaticObject> mPotTop;
	std::unique_ptr<Ingredient> mRedPotion;
	std::unique_ptr<Ingredient> mBluePotion;
	std::unique_ptr<Ingredient> mYellowPotion;

	std::unique_ptr<StaticObject> mWall;
	std::unique_ptr<StaticObject> mWindow;
	std::unique_ptr<Square> mTable;
	std::unique_ptr<Water> mWater;


	std::unique_ptr<MovableStaticObject>  mSplash;	//色表示UI
	std::unique_ptr<MovableStaticObject> mMissionPaper;		//色表示背景UI
	std::unique_ptr<MovableStaticObject> mEnvelope;
	std::unique_ptr<D2D_UIStackContainer> mText;	//詳しい文字

	std::unique_ptr<CandleLight> mCandleLight1;
	std::unique_ptr<CandleLight> mCandleLight2;

	std::unique_ptr<UIGoldDisplay> mGoldDisplay;

	//シーン切り替え用
	SceneConfig::SceneIndex mNextScene = SceneConfig::SceneIndex::SCENE_NONE;

	//カメラ切り替えトリガー
	bool isCheckMission = false;
	//文字UIが後回しするかどうか
	bool isTextActiveDelay = false;

public:
	/// @brief データのロードと初期化
	void Init();

	/// @brief データのセーブを行う
	void UnInit();

	void Update(float dt);

	void Draw();

protected:

	void GameObjectUpdate(float dt);
	void TriggerListener();

	void InitShadowRenderTarget();
	
	void DrawObjectsWithShadow();
	void DrawLeftObjectWithShadow();
	void DrawRightObjectWithShadow();
	void DrawMiddleObjectWithShadow();


};


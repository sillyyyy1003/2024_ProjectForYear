#pragma once
#include "CandleLight.h"
#include "Cylinder.h"
#include "InteractiveStaticObject.h"
#include "MissionPaper.h"
#include "PointLight.h"
#include "SceneBase.h"
#include "Sphere.h"
#include "StaticObject.h"

#include "Water.h"

class SceneLab :public SceneBase
{

	//Texture List
	PBRConfig::PBRTexList pbrTexList;


	std::unordered_map<std::string, StaticObject*> staticObjList;

	std::unique_ptr<InteractiveStaticObject> mPot;
	std::unique_ptr<InteractiveStaticObject> mRedBook;
	std::unique_ptr<InteractiveStaticObject> mBlueBook;


	std::unique_ptr<Square> mTable;

	std::unique_ptr<Water> mWater;
	std::unique_ptr<MissionPaper> mMissionPaper;

	std::unique_ptr<Cube> mCube;
	std::unique_ptr<Cylinder> mCylinder;

	std::unique_ptr<Sphere> mLightBulb2;

	//std::unique_ptr<PointLight> pointLight;

	std::unique_ptr<CandleLight> mCandleLight;

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

};


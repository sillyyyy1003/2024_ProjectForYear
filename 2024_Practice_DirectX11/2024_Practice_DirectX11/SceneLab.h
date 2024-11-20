#pragma once
#include "Cylinder.h"
#include "InteractiveStaticObject.h"
#include "MissionPaper.h"
#include "SceneBase.h"
#include "StaticObject.h"
#include "Tile.h"
#include "Water.h"

class SceneLab :public SceneBase
{

	std::unordered_map<std::string, std::shared_ptr<StaticObject>> staticObjList;

	std::shared_ptr<InteractiveStaticObject> mPot;
	PBRConfig::PBRTexList pbrTexList;

	std::unique_ptr<Square> mTable;

	std::unique_ptr<Water> mWater;
	std::unique_ptr<MissionPaper> testPaper;

	std::unique_ptr<Cube> mCube;
	std::unique_ptr<Cylinder> mCylinder;
	
	bool m_isMulti;
	int m_index;
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

	void InitTestObj();
	void DrawTestObj();

};


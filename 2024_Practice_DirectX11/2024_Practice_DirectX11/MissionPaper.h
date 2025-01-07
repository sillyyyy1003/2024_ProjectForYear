#pragma once
#include "D2D_UIStackContainer.h"
#include "SceneBase.h"
#include "Square.h"
#include "StaticObject.h"

namespace Paper
{
	enum PaperState :UINT
	{

		STATE_FALLING = 1 << 0,		//壁から降りる状態
		STATE_DEFAULT = 1 << 1,		//待機状態
		STATE_ZOOM = 1 << 2,			//詳しい文字を表示する
		STATE_DISAPPEAR = 1 << 3,	//Missionが選択されてない状態

	};


}


/// @brief 机の上に置いてある
class MissionPaper :public Component
{
	std::unique_ptr<Square> mPaperMesh;//紙を描画する
	std::unique_ptr<Square> mSplash;	//色表示用
	std::unique_ptr<Square> mText;		//省略文字
	std::unique_ptr<D2D_UIStackContainer> m2DFont;	//表示文字
	//主要功能，描绘纸张和颜色。
	//1.远处观看时，显示纸张和任务颜色还有省略文字
	//2.近处观看时，显示任务内容，具体文字和颜色

	bool isShowText = false;
	DirectX::XMFLOAT3 mPosition = {};	//紙の描画位置
	DirectX::XMFLOAT3 mRotation = {};
	DirectX::XMFLOAT3 mScale = { 1.f,1.f,1.f };

	UINT mState = 0;	//紙の状態

public:
	MissionPaper();
	~MissionPaper() override = default;

	/// @brief
	void InitMissionPaper();
	void Update(float dt);
	void Draw();


	//SaveLoad機能
	void LoadSaveData(json data);
	json SaveData();

	void PreUpdate(float dt);
	void GameUpdate(float dt);
	void LateUpdate(float dt);

	void SetState(Paper::PaperState state);
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetRotation(DirectX::XMFLOAT3 rotation);


};


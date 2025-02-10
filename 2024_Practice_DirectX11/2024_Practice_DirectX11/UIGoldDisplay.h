#pragma once
#include "D2D_UIStackContainer.h"
#include "SceneBase.h"

class Player;

//プレイヤーのコイン数を表示する
class UIGoldDisplay :public Component
{
	//CoinIcon表示

	//数字表示
	std::unique_ptr<D2D_UIStackContainer> mGoldUI = nullptr;
	//プレイヤー
	Player* mPlayer = nullptr;

	int mState = 0;	//UIの状態
	float mAccumulateTime = 0.0f;
	float mDuration = 0.0f;

	int mStartNum = 0;
	int mIncreaseNum = 0;	//増やしたお金
	int mIncreaseResult = 0;

public:
	UIGoldDisplay() {};
	~UIGoldDisplay() override = default;


	void Init(Player* player,D2DUIConfig::FontSize fontSize);

	void Update(float dt);

	void Draw();

	/// @brief UIのフォーマット設定データ読み込み
	/// @param data 
	void LoadSceneData(const json& data);

	//UIのフォーマット設定データ書き出し
	json SaveData();

	void SetIncreaseFunction(float duration, int increaseNum);
protected:

	void OnStateNone(float dt);
	void OnStateCoinNumChange(float dt);
	
};


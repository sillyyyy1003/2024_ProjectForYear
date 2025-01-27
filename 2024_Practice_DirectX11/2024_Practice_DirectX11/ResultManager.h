#pragma once
#include <memory>
#include <FastNoise/FastNoiseLite.h>

#include "D2D_UIStackContainer.h"
#include "MissionManager.h"

namespace ResultConfig
{
	constexpr float HUE_WEIGHT = 1.f;//結果計算するとの比重
	constexpr float SATURATION_WEIGHT = 0.5f;//結果計算するとの比重
	constexpr float VALUE_WEIGHT = 0.5f;//結果計算するとの比重

}

class ResultManager
{
private:
	//結果の描画
	std::unique_ptr<D2D_UIStackContainer> mResult;
	std::unique_ptr<UI_Square> mResultBackGround;
	//結果表示するか？
	bool isActive = false;

	bool isGenerateResult = false;
	//結果の文字
	std::string mResultContents;	

	ResultManager() {};
	~ResultManager() = default;

	float mShadowRadius = 0.0f;
	float mAccumulateTime = 0.0f;
	float mChangeRange = 0.0f;	//変化範囲（基準値0.0001)

public:
	static ResultManager* Get() { static ResultManager instance; return &instance; };

	/// @brief データ初期化
	void Init();

	void Update(float dt);

	void Draw();

	void ResultGenerator();

	void GenerateCurrentResult();

	/// @brief リザルト表示
	void SetActive(bool isActive) { this->isActive = isActive; };
	bool GetActive() { return isActive; };

	json SaveData();
	void LoadSaveData(json data);

};


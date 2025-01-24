#pragma once
#include <memory>

#include "D2D_UIStackContainer.h"
#include "MissionManager.h"

class ResultManager
{
private:
	//結果の描画
	std::unique_ptr<D2D_UIStackContainer> mResult;
	//結果表示するか？
	bool isActive = false;

	bool isGenerateResult = false;

	std::string mResult;

	ResultManager() {};
	~ResultManager() = default;

public:
	static ResultManager* Get() { static ResultManager instance; return &instance; };

	/// @brief データ初期化
	void Init();

	void Update(float dt);

	void Draw();

	void ResultGenerator();

	void GenerateCurrentResult();

};


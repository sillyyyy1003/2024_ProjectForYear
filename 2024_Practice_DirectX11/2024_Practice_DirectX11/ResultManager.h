#pragma once
#include <memory>
#include <FastNoise/FastNoiseLite.h>

#include "D2D_UIStackContainer.h"
#include "MissionManager.h"

namespace ResultConfig
{
	constexpr float HUE_WEIGHT = 1.f;//���ʌv�Z����Ƃ̔�d
	constexpr float SATURATION_WEIGHT = 0.5f;//���ʌv�Z����Ƃ̔�d
	constexpr float VALUE_WEIGHT = 0.5f;//���ʌv�Z����Ƃ̔�d

}

class ResultManager
{
private:
	//���ʂ̕`��
	std::unique_ptr<D2D_UIStackContainer> mResult;
	std::unique_ptr<UI_Square> mResultBackGround;
	//���ʕ\�����邩�H
	bool isActive = false;

	bool isGenerateResult = false;
	//���ʂ̕���
	std::string mResultContents;	

	ResultManager() {};
	~ResultManager() = default;

	float mShadowRadius = 0.0f;
	float mAccumulateTime = 0.0f;
	float mChangeRange = 0.0f;	//�ω��͈́i��l0.0001)

public:
	static ResultManager* Get() { static ResultManager instance; return &instance; };

	/// @brief �f�[�^������
	void Init();

	void Update(float dt);

	void Draw();

	void ResultGenerator();

	void GenerateCurrentResult();

	/// @brief ���U���g�\��
	void SetActive(bool isActive) { this->isActive = isActive; };
	bool GetActive() { return isActive; };

	json SaveData();
	void LoadSaveData(json data);

};


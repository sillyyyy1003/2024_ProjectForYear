#pragma once
#include "FirstPersonCamera.h"
#include "PotionEffect.h"
#include "Water.h"

class Potion :public Water
{
private:

	float mDiluteParam = 0.0f;	//水的稀释程度
	float mAccumulateTime = 0.0f;

	bool isAutoColor = false;	//自動色変換
	float mColorTransformSpeed;	//自動色変換速度

	float mPreviousHue = -1;		//前回入れた色
	float mValue = 0.0f;			//ミックスによる色深くなるパラメータ
	float accumulateValue;

	float mAccumulateHeight = 0.0f;	//
	float mDefaultHeight = 0.0f;	//初期高さ

	float mMaxHeight = 0.f;
	float mLimitHeight = 0.0f;

	int mWaterLevelState = 0;

	//Effectに関わる変数
	int mParticleNum = 500;

	FirstPersonCamera* mCamera = nullptr;


protected:

	std::unique_ptr<PotionEffect> mParticleEffect;

public:

	Potion() = default;
	~Potion() override = default;


	void Update(float dt) override;
	void Draw() override;

	/// @brief Dilute by Water
	/// @param diluteAlpha 入れた水の量
	void Dilute(float diluteAlpha);

	/// @brief 色混ぜ
	/// @param color 
	/// @param alpha 
	void MixColor(DirectX::XMFLOAT4 color, float alpha);

	/// @brief 色混ぜ具体の手取
	/// @param baseColor Potionの色
	/// @param mixColor 入れた顔料の色
	/// @param alpha 入れた量
	/// @return 混ぜたあとの色
	HSV BlendColor(const HSV& baseColor, const HSV& mixColor, float alpha);

	void AutoColor(float dt);

	void SetAutoColor(bool isAutoColour) { this->isAutoColor = isAutoColour; };
	void SetWaterColor(DirectX::XMFLOAT4 color);

	void LoadSaveData(json data, const char* objName) override;
	json SaveData() override;

	void SetPreviousHue(float hue);

	/// @brief 水位が上昇する
	/// @param param 上昇スピード
	void RiseUpWaterLevel(float param);

	/// @brief 水位に関するパラメータの初期化
	void InitWaterLevelParam();

	/// @brief ポーションのエフェクトの初期化を行う
	void InitPotionParticleEffect();

	void InitPotionParticleEffect(DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 accel, float lifeTime, int particleNum, float size);

	/// @brief カメラに渡す
	float CalculateCurrentShakingParam();

	/// @brief 水位をリセットする
	void ResetWaterLevel();

	DirectX::XMFLOAT4 GetPotionColor() { return mModel->GetMaterial().diffuse; };

	/// @brief パーティクルの数を設定する
	/// @param num 
	void SetParticleNum(int num) { mParticleNum = num; };

	/// @brief 揺らすカメラを設定
	/// @param camera 
	void SetCamera(FirstPersonCamera* camera);

	/// @brief ポーションすべての状態をリセットする
	void ResetPotion();

};

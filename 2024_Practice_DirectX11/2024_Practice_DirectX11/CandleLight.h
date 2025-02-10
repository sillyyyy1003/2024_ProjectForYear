#pragma once
#include <FastNoise/FastNoiseLite.h>

#include "FireEffect.h"
#include "PointLight.h"
#include "Sphere.h"



class CandleLight :public PointLight
{
private:
#ifdef _DEBUG
	std::string name = "default";
#endif
	
	bool isShaking = true;
	Light::PointLight mCandleLight;		//オブジェクトを照らす用
	DirectX::XMFLOAT3 mCastShadowLightPos = { 0,1,0 };	//Cast Shadow用の
	float mCastShadowHeight = 0.0f;
	float mTime = 0.0;

	FastNoiseLite mCandleNoise;	//ランダムウンド用

	std::unique_ptr<FireEffect> mEffect;
	DirectX::XMFLOAT3 mFireOffset = { 0,-0.3f,0 };

	
public:

	CandleLight();
	~CandleLight();

	void Init();
	void InitName(const char* name);

	/// @brief イフェクトの初期化
	/// @param emitVelocity 初期速度
	/// @param emitAccel 初期加速度
	/// @param time 粒子存続時間
	/// @param startColor 
	/// @param endColor 
	void InitEffect(DirectX::XMFLOAT3 emitVelocity, DirectX::XMFLOAT3 emitAccel, float time, DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor);
	void InitEffect(json data);

	void Update(float dt) override;
	void CandleLightShaking(float dt);
	void Draw();

	const Light::PointLight& GetPointLight();
	const DirectX::XMFLOAT3& GetCastShadowLightPos() const { return mCastShadowLightPos; };

	json SaveData() override;
	void LoadSaveData(json data, const char* objName) override;


};


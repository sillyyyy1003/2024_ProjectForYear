﻿#pragma once
#include <FastNoise/FastNoiseLite.h>

#include "PointLight.h"
#include "Sphere.h"

class CandleLight :public PointLight
{
private:
#ifdef _DEBUG
	std::unique_ptr<Sphere> mDebugMesh;
	std::string name = "default";
#endif
	
	bool isShaking = true;
	Light::PointLight mCandleLight;		//オブジェクトを照らす用
	DirectX::XMFLOAT3 mCastShadowLightPos = { 0,1,0 };	//Cast Shadow用の
	float mCastShadowHeight = 0.0f;
	float mTime = 0.0;

	FastNoiseLite mCandleNoise;	//ランダムウンド用
public:

	CandleLight();
	~CandleLight();

	void Init();
	void InitName(const char* name);

	void Update(float dt) override;
	void CandleLightShaking(float dt);
	void Draw();

	const Light::PointLight& GetPointLight();
	const DirectX::XMFLOAT3& GetCastShadowLightPos() const { return mCastShadowLightPos; };

	json SaveData() override;
	void LoadSaveData(json data, const char* objName) override;

};


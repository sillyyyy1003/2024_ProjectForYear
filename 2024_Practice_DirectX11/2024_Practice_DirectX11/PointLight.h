#pragma once
#include "CameraBase.h"
#include "LightBase.h"

class PointLight : public LightBase
{
protected:
	float mRange = 0.0f;					//LightÇÃîÕàÕ
	DirectX::XMFLOAT3  mAttenuation = {};	//å∏êä

public:

	PointLight();
	~PointLight() override = default;

	void Update(float dt) override;

	float GetRange() { return mRange; };
	const DirectX::XMFLOAT3& GetAttenuation() const noexcept { return mAttenuation; };


	void SetRange(float _range);
	void SetAttenuation(const DirectX::XMFLOAT3& _attenuation);
	void SetAttenuation(float x, float y, float z);
	void SetAttenuation(const float* attenuation);

	const Light::PointLight& GetPointLight() const;
	json SaveData();

	void LoadSaveData(json data, const char* objName);



};


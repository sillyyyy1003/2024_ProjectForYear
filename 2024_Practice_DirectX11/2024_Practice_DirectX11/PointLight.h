#pragma once
#include "CameraBase.h"
#include "LightBase.h"

class PointLight : public LightBase
{
protected:
	float mRange = 0.0f;					//Lightの範囲
	DirectX::XMFLOAT3  mAttenuation = {};	//減衰

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

	Light::PointLight GetPointLight();
	virtual json SaveData();

	virtual void LoadSaveData(json data, const char* objName);



};


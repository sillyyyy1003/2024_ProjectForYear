#pragma once
#include "CameraBase.h"
#include "LightBase.h"

class PointLight : public LightBase
{

	float mRange = 0.0f;					//Light‚Ì”ÍˆÍ
	DirectX::XMFLOAT3  mAttenuation = {};	//Œ¸Š

public:

	PointLight();
	~PointLight() override = default;

	void Update(float dt) override;

	float GetRange() { return mRange; };
	const DirectX::XMFLOAT3& GetAttenuation()noexcept { return mAttenuation; };


	void SetRange(float _range);
	void SetAttenuation(const DirectX::XMFLOAT3& _attenuation);
	void SetAttenuation(float x, float y, float z);
	void SetAttenuation(const float* attenuation);


};


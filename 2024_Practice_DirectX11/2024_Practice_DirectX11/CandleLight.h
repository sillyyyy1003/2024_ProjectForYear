#pragma once
#include "PointLight.h"
#include "Sphere.h"

class CandleLight :public PointLight
{
private:

	std::unique_ptr<Sphere> mDebugMesh;
	bool isShaking = true;
	Light::PointLight mCandleLight;		//
	DirectX::XMFLOAT3 mCastShadowLightPos={0,1,0};	//CastLightPos
public:

	CandleLight();
	~CandleLight();

	void Init();
	void Update(float dt) override;
	void CandleLightShaking(float dt);
	void Draw();

	const Light::PointLight& GetPointLight();
	const DirectX::XMFLOAT3& GetCastShadowLightPos() const { return mCastShadowLightPos; };
};


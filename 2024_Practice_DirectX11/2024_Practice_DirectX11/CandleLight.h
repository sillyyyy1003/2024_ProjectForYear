#pragma once
#include "PointLight.h"
#include "Sphere.h"

class CandleLight :public PointLight
{
private:

	std::unique_ptr<Sphere> mDebugMesh;
	
	bool isShaking = true;
	Light::PointLight mCandleLight;		//�I�u�W�F�N�g���Ƃ炷�p
	DirectX::XMFLOAT3 mCastShadowLightPos = { 0,1,0 };	//Cast Shadow�p��
	float mCastShadowHeight = 0.0f;
public:

	CandleLight();
	~CandleLight();

	void Init();
	void Update(float dt) override;
	void CandleLightShaking(float dt);
	void Draw();

	const Light::PointLight& GetPointLight();
	const DirectX::XMFLOAT3& GetCastShadowLightPos() const { return mCastShadowLightPos; };

	json SaveData() override;
	void LoadSaveData(json data, const char* objName) override;

};


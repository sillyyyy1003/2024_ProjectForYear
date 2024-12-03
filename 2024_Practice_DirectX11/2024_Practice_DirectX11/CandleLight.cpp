#include "CandleLight.h"

#include "GampApp.h"

CandleLight::CandleLight()
{
}

CandleLight::~CandleLight()
{
}

void CandleLight::Init()
{

    mDebugMesh = std::make_unique<Sphere>();
    mDebugMesh->Init(nullptr);
    mDebugMesh->SetDiffuse({ 1,0,0,1 });
    mDebugMesh->SetScale(0.5, 0.5, 0.5);
    mDebugMesh->LoadDefShader();
	GameApp::GetNoise().SetFrequency(0.5f);
}

void CandleLight::Update(float dt)
{
#ifdef _DEBUG
	if (ImGui::Begin("Candle Light Option"))
	{
		ImGui::Checkbox("isEnable", &isEnable);

		ImGui::Checkbox("isShaking", &isShaking);

		float ambient[4] = {
			GetAmbient().x,GetAmbient().y,GetAmbient().z,GetAmbient().w
		};
		ImGui::ColorEdit4("Ambient", ambient);
		SetAmbient(ambient);

		float diffuse[4] = {
		GetDiffuse().x,GetDiffuse().y,GetDiffuse().z,GetDiffuse().w
		};
		ImGui::ColorEdit4("Diffuse", diffuse);
		SetDiffuse(diffuse);

		//位置
		float pos[3] = { mPos.x,mPos.y,mPos.z };
		ImGui::InputFloat3("Position", pos);
		SetPosition(pos);

		//減衰
		float attenuation[3] = { mAttenuation.x,mAttenuation.y,mAttenuation.z };
		ImGui::InputFloat3("Attenuation", attenuation);
		SetAttenuation(attenuation);

		//範囲設定
		ImGui::SliderFloat("Range", &mRange, 0, 40.f);
	}

	ImGui::End();
#endif
    CandleLightShaking(dt);
}

void CandleLight::CandleLightShaking(float dt)
{
    if (!isShaking)return;
    static float time = 0;
    time += dt;

	Vector3 basePos = GetPosition();
    float flickerX = GameApp::GetNoise().GetNoise(time, 0.0f) * 0.15f; // X 方向偏移
    float flickerY = GameApp::GetNoise().GetNoise(time, 1.0f) * 0.05f; // Y 方向偏移
    float flickerZ = GameApp::GetNoise().GetNoise(time, 2.0f) * 0.15f; // Z 方向偏移

	// 动态调整范围
    float baseRange = GetRange();

    // 动态调整亮度
    float baseIntensity = GetAttenuation().x;
    float intensityFlicker = GameApp::GetNoise().GetNoise(time, 4.0f) * 0.25f; // 添加轻微变化
  
    mCandleLight.ambient = GetAmbient();
    mCandleLight.diffuse = GetDiffuse();
    mCandleLight.position= basePos + Vector3(flickerX, 0, flickerZ);
    mCandleLight.range = baseRange + GameApp::GetNoise().GetNoise(time, 3.0f) * 2.f;
    mCandleLight.attenuation = { baseIntensity + intensityFlicker, 0, 0 };
    mCandleLight.isEnable = true;
    mCastShadowLightPos = mCandleLight.position;
    mCastShadowLightPos.y = mCandleLight.position.y + 6.f;


    mDebugMesh->SetPosition(mCandleLight.position);


}

void CandleLight::Draw()
{
    mDebugMesh->Draw();
}

const Light::PointLight& CandleLight::GetPointLight()
{
    return mCandleLight;
}

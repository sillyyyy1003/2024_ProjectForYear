#include "CandleLight.h"
#include "GameApp.h"

CandleLight::CandleLight()
{
}

CandleLight::~CandleLight()
{
}

void CandleLight::Init()
{
#ifdef _DEBUG
    mDebugMesh = std::make_unique<Sphere>();
    mDebugMesh->Init(nullptr);
    mDebugMesh->SetDiffuse({ 1,0,0,1 });
    mDebugMesh->SetScale(0.5, 0.5, 0.5);
    mDebugMesh->LoadDefShader();
#endif

	GameApp::GetNoise().SetFrequency(0.5f);
}

void CandleLight::InitName(const char* name)
{
	this->name = name;
}

void CandleLight::Update(float dt)
{
#ifdef _DEBUG
	if (ImGui::Begin(name.c_str()))
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

		ImGui::InputFloat("CastShadowHeight", &mCastShadowHeight);
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
    float intensityFlicker = GameApp::GetNoise().GetNoise(time, 4.0f) * 0.15f; // 添加轻微变化
  
    mCandleLight.ambient = GetAmbient();
    mCandleLight.diffuse = GetDiffuse();
    mCandleLight.position= basePos + Vector3(flickerX, 0, flickerZ);
    mCandleLight.range = baseRange + GameApp::GetNoise().GetNoise(time, 3.0f) * 1.f;
    mCandleLight.attenuation = { baseIntensity + intensityFlicker, 0, 0 };
    mCandleLight.isEnable = true;
    mCastShadowLightPos = mCandleLight.position;
    mCastShadowLightPos.y = mCandleLight.position.y + mCastShadowHeight;

#ifdef _DEBUG
    mDebugMesh->SetPosition(mCandleLight.position);
#endif

}

void CandleLight::Draw()
{
	#ifdef _DEBUG
    mDebugMesh->Draw();
#endif

}

const Light::PointLight& CandleLight::GetPointLight()
{
    return mCandleLight;
}

json CandleLight::SaveData()
{
	json data;
	data["Position"] = { GetPosition().x,GetPosition().y,GetPosition().z };
	data["Direction"] = { GetDirection().x,GetDirection().y,GetDirection().z };

	data["Ambient"] = { GetAmbient().x, GetAmbient().y, GetAmbient().z, GetAmbient().w };
	data["Diffuse"] = { GetDiffuse().x,GetDiffuse().y, GetDiffuse().z, GetDiffuse().w };

	data["Attenuation"] = { GetAttenuation().x,GetAttenuation().y,GetAttenuation().z };
	data["Range"] = { GetRange() };
	data["CastHeight"] = { mCastShadowHeight };
	return data;
}

void CandleLight::LoadSaveData(json data, const char* objName)
{
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	SetPosition(pos);

	//Init Ambient
	Color ambient = Color(data[objName]["Ambient"][0], data[objName]["Ambient"][1], data[objName]["Ambient"][2], data[objName]["Ambient"][3]);
	SetAmbient(ambient);

	//Init Diffuse
	Color diffuse = Color(data[objName]["Diffuse"][0], data[objName]["Diffuse"][1], data[objName]["Diffuse"][2], data[objName]["Diffuse"][3]);
	SetDiffuse(diffuse);

	//Init Attenuation
	Vector3 attenuation = Vector3(data[objName]["Attenuation"][0], data[objName]["Attenuation"][1], data[objName]["Attenuation"][2]);
	SetAttenuation(attenuation);

	float range = data[objName]["Range"][0];
	SetRange(range);

	mCastShadowHeight = data[objName]["CastHeight"][0];
}

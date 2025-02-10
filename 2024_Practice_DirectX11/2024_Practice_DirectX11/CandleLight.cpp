#include "CandleLight.h"
#include "GameApp.h"
#include "RandomGenerator.h"
#include "RenderState.h"

CandleLight::CandleLight()
{
}

CandleLight::~CandleLight()
{
}

void CandleLight::Init()
{
	mCandleNoise.SetSeed(RandomGenerator::Get()->RandomInt(0, 100));
	mCandleNoise.SetFrequency(0.3f);
	mCandleNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

void CandleLight::InitName(const char* name)
{
#ifdef _DEBUG
	this->name = name;
#endif
}



void CandleLight::InitEffect(DirectX::XMFLOAT3 emitVelocity, DirectX::XMFLOAT3 emitAccel, float time,
	DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor)
{

	//Effect初期化
	mEffect = std::make_unique<FireEffect>();

	//todo:これを数値として保存する
	mEffect->InitParticleRenderer(1500,0.015f);
	mEffect->InitFireParticleData(mCandleLight.position + mFireOffset, emitAccel, emitVelocity, time);
	mEffect->SetParticleColorRange(startColor, endColor);
	

}

void CandleLight::InitEffect(json data)
{
	mEffect = std::make_unique<FireEffect>();
	float particleLife = { data["ParticleLife"] };
	Vector3 particleEmitVelocity(data["ParticleEmitVel"][0], data["ParticleEmitVel"][1], data["ParticleEmitVel"][2]);
	Vector3 particleEmitAccel(data["ParticleEmitAccel"][0], data["ParticleEmitAccel"][1], data["ParticleEmitAccel"][2]);
	int particleNum = { data["ParticleNum"] };
	float particleSize = { data["ParticleSize"] };
	Color startColor(data["StartColor"][0], data["StartColor"][1], data["StartColor"][2], data["StartColor"][3]);
	Color endColor(data["EndColor"][0], data["EndColor"][1], data["EndColor"][2], data["EndColor"][3]);


	mEffect->InitParticleRenderer(particleNum,particleSize);
	mEffect->InitFireParticleData(mCandleLight.position + mFireOffset, particleEmitAccel, particleEmitVelocity, particleLife);
	mEffect->SetParticleColorRange(startColor, endColor);
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

	if(mEffect)
	{
		mEffect->UpdateEmitPos(mCandleLight.position + mFireOffset);
		mEffect->Update(dt);
	}
}

void CandleLight::CandleLightShaking(float dt)
{
    if (!isShaking)return;

	mTime += dt;

	//パーリンノイズを位置に導入
	Vector3 basePos = GetPosition();
    float flickerX = mCandleNoise.GetNoise(mTime, 0.0f) * 0.1f; // X 方向
    float flickerZ = mCandleNoise.GetNoise(mTime, 2.0f) * 0.1f; // Z 方向

	// 光の範囲を取得
    float baseRange = GetRange();

    // ライトの強度にノイズ入れ
    float baseIntensity = GetAttenuation().x;
    float intensityFlicker = mCandleNoise.GetNoise(mTime, 4.0f) * 0.15f;

    mCandleLight.ambient = GetAmbient();
    mCandleLight.diffuse = GetDiffuse();
    mCandleLight.position= basePos + Vector3(flickerX, 0, flickerZ);
    mCandleLight.range = baseRange + mCandleNoise.GetNoise(mTime, 3.0f) * 1.f;
    mCandleLight.attenuation = { baseIntensity + intensityFlicker, 0, 0 };
    mCandleLight.isEnable = true;
    mCastShadowLightPos = mCandleLight.position;
    mCastShadowLightPos.y = mCandleLight.position.y + mCastShadowHeight;


}

void CandleLight::Draw()
{
	GameApp::SetBlendState(RenderState::BSAlphaWeightedAdditive);
	if (mEffect)
		mEffect->Draw();
	GameApp::SetBlendState(nullptr);
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

	if (!mEffect)return data;

	//FireEffectに関するデータ
	data["ParticleNum"] = mEffect->GetParticleNum();
	data["ParticleSize"] = mEffect->GetParticleSize();
	data["StartColor"] = { mEffect->GetStartColor().x,mEffect->GetStartColor().y,mEffect->GetStartColor().z,mEffect->GetStartColor().w };
	data["EndColor"] = { mEffect->GetEndColor().x,mEffect->GetEndColor().y,mEffect->GetEndColor().z,mEffect->GetEndColor().w };
	data["ParticleEmitVel"]={mEffect->GetEmitVelocity().x,mEffect->GetEmitVelocity().y,mEffect->GetEmitVelocity().z};
	data["ParticleEmitAccel"]={mEffect->GetEmitAcceleration().x,mEffect->GetEmitAcceleration().y,mEffect->GetEmitAcceleration().z};
	data["ParticleLife"] = mEffect->GetParticleLife() ;
	
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

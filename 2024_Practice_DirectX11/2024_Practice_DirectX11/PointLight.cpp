#include "PointLight.h"

PointLight::PointLight()
{
}

void PointLight::Update(float dt)
{
#ifdef _DEBUG
	if (ImGui::Begin("Point Light Option"))
	{
		ImGui::Checkbox("isEnable", &isEnable);

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

		//ˆÊ’u
		float pos[3] = { mPos.x,mPos.y,mPos.z};
		ImGui::InputFloat3("Position", pos);
		SetPosition(pos);

		//Œ¸Š
		float attenuation[3] = { mAttenuation.x,mAttenuation.y,mAttenuation.z };
		ImGui::InputFloat3("Attenuation", attenuation);
		SetAttenuation(attenuation);

		//”ÍˆÍÝ’è
		ImGui::SliderFloat("Range", &mRange, 0, 40.f);
	}

	ImGui::End();
#endif

}

void PointLight::SetRange(float _range)
{
	mRange = _range;
}

void PointLight::SetAttenuation(const DirectX::XMFLOAT3& _attenuation)
{
	mAttenuation = _attenuation;
}

void PointLight::SetAttenuation(float x, float y, float z)
{
	mAttenuation = Vector3(x, y, z);
}

void PointLight::SetAttenuation(const float* attenuation)
{
	mAttenuation = Vector3(attenuation[0], attenuation[1], attenuation[2]);
}

const Light::PointLight& PointLight::GetPointLight() const
{
	Light::PointLight light;
	light = {
		this->GetAmbient(),
		this->GetDiffuse(),
		this->GetPosition(),
		mRange,
		this->GetAttenuation(),
		true,
	};
	return light;
}

json PointLight::SaveData()
{
	json data;
	data["Position"] = { GetPosition().x,GetPosition().y,GetPosition().z };
	data["Direction"] = { GetDirection().x,GetDirection().y,GetDirection().z };

	data["Ambient"] = { GetAmbient().x, GetAmbient().y, GetAmbient().z, GetAmbient().w };
	data["Diffuse"] = { GetDiffuse().x,GetDiffuse().y, GetDiffuse().z, GetDiffuse().w };

	data["Attenuation"] = { GetAttenuation().x,GetAttenuation().y,GetAttenuation().z };
	data["Range"] = { GetRange() };

	return data;
}

void PointLight::LoadSaveData(json data, const char* objName)
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
}

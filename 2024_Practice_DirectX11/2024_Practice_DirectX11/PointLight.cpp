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
		ImGui::SliderFloat("Range", &mRange, 0, 20.f);
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

const Light::PointLight& PointLight::GetPointLight()
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

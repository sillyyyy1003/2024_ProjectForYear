#include "DirLight.h"
#include <imgui/Include/imgui.h>

void DirLight::Update(float dt)
{
#ifdef _DEBUG
	if(ImGui::Begin("Light Option"))
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


		float pos[3] = {
		mPos.x,mPos.y,mPos.z
		};
		ImGui::InputFloat3("Position", pos);
		SetPosition(pos);

		float dir[3] = {
		mDir.x,mDir.y,mDir.z };
		ImGui::InputFloat3("Direction", dir);
		SetDirection(dir);
	}

	ImGui::End();
#endif
}

void DirLight::LoadSaveData(json data, const char* objName)
{
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	SetPosition(pos);

	//Init direction
	Vector3 direction = Vector3(data[objName]["Direction"][0], data[objName]["Direction"][1], data[objName]["Direction"][2]);
	SetDirection(direction);

	//Init Ambient
	Color ambient = Color(data[objName]["Ambient"][0], data[objName]["Ambient"][1], data[objName]["Ambient"][2], data[objName]["Ambient"][3]);
	SetAmbient(ambient);

	//Init Diffuse
	Color diffuse = Color(data[objName]["Diffuse"][0], data[objName]["Diffuse"][1], data[objName]["Diffuse"][2], data[objName]["Diffuse"][3]);
	SetDiffuse(diffuse);

}

json DirLight::SaveData()
{
	json data;
	data["Position"] = { GetPosition().x,GetPosition().y,GetPosition().z };
	data["Direction"] = { GetDirection().x,GetDirection().y,GetDirection().z };
	data["Ambient"] = { GetAmbient().x, GetAmbient().y, GetAmbient().z, GetAmbient().w };
	data["Diffuse"] = { GetDiffuse().x,GetDiffuse().y, GetDiffuse().z, GetDiffuse().w };


	return data;
}

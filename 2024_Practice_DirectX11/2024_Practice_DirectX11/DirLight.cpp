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
		SetPos(pos);

		float dir[3] = {
		mDir.x,mDir.y,mDir.z };
		ImGui::InputFloat3("Direction", dir);
		SetDir(dir);
	}

	ImGui::End();
#endif
}

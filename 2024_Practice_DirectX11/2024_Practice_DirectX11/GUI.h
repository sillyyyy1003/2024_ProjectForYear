#pragma once
#include "D3DApp.h"


class GUI
{
private:
	GUI();
	~GUI() = default;
	static GUI& GetInstance();

public:
	/// @brief 初期化
	static void Init();

	
	/// @brief 色表示
	static void ShowColor(const DirectX::XMFLOAT4& _color);

	/// @brief 位置表示
	static void ShowFloat3(const DirectX::XMFLOAT3& _Pos);

	static void ShowFloat4(const DirectX::XMFLOAT4& _rot);

	template<class T>
	static void ObjectSetting(T* obj, const char* windowName);

};

template <class T>
void GUI::ObjectSetting(T* obj, const char* windowName)
{
	if (ImGui::Begin(windowName))
	{
		//Set Pos
		float Pos[3] = { obj->GetPosition().x,obj->GetPosition().y,obj->GetPosition().z };
		//ImGui::InputFloat3("Position",Pos);
		ImGui::SliderFloat3("Position", Pos, 0, 20);
		obj->SetPosition(Pos);


		float Scale[3] = { obj->GetScale().x,obj->GetScale().y,obj->GetScale().z };
		ImGui::InputFloat3("Scale", Scale);
		obj->SetScale(Scale);
	}

	ImGui::End();
	//Set Scale
}

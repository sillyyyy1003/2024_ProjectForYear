#include "GUI.h"


GUI::GUI()
{
	GUI::GetInstance().Init();
}

GUI& GUI::GetInstance()
{
	static GUI instance;
	return instance;
}

void GUI::Init()
{

}

void GUI::ShowColor(const DirectX::XMFLOAT4& _color)
{
	ImGui::Text("Color : r: %2f  g: %2f  b: %2f a: %2f", _color.x, _color.y, _color.z, _color.w);
}

void GUI::ShowFloat3(const DirectX::XMFLOAT3& _pos)
{
	ImGui::Text("Float3 : x: %2f  y: %2f  z: %2f", _pos.x, _pos.y, _pos.z);
}

void GUI::ShowFloat4(const DirectX::XMFLOAT4& _rot)
{
	ImGui::Text("Float4 : x: %2f  y: %2f  z: %2f  w: %2f", _rot.x, _rot.y, _rot.z, _rot.w);
}



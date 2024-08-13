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

void GUI::ShowsFloat3(const DirectX::XMFLOAT3& _pos)
{
	ImGui::Text("Float3 : x: %2f  y: %2f  z: %2f", _pos.x, _pos.y, _pos.z);
}

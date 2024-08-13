#pragma once
#include "D3DApp.h"

class GUI
{
private:
	GUI();
	~GUI() = default;
	static GUI& GetInstance();

public:
	/// @brief èâä˙âª
	static void Init();

	static void ShowColor(const DirectX::XMFLOAT4& _color);
	static void ShowsFloat3(const DirectX::XMFLOAT3& _Pos);



};
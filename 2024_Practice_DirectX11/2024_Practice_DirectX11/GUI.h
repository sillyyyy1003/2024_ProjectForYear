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


};
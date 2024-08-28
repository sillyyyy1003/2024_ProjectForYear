#pragma once
#include "D3DApp.h"


class GUI
{
private:
	GUI();
	~GUI() = default;
	static GUI& GetInstance();

public:
	/// @brief ������
	static void Init();

	
	/// @brief �F�\��
	static void ShowColor(const DirectX::XMFLOAT4& _color);

	/// @brief �ʒu�\��
	static void ShowsFloat3(const DirectX::XMFLOAT3& _Pos);



};
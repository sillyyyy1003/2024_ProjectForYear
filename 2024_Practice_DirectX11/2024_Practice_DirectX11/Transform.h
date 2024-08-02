#pragma once
#include "D3DUtil.h"

///位置、回転、大きさなどの管理
class Transform
{
public:
	DirectX::XMFLOAT3 mPos = {};
	DirectX::XMFLOAT3 mRotation = {};
	DirectX::XMFLOAT3 mScale = {1.f,1.f,1.f};

private:
	DirectX::XMFLOAT4X4 mMatrix = {};//World Matrix


public:
	Transform() = default;
	~Transform() = default;

	/// @brief データを行列に変換
	/// @return 行列を返す
	DirectX::XMFLOAT4X4 GetMatrix();

	void Update();

private:
	/// @brief Transformデータを行列に変換する
	void GenerateWorld();
};


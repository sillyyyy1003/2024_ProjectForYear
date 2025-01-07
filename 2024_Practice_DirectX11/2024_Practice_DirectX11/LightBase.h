#pragma once
#include <DirectXCollision.h>

/// @brief 光基底クラス
class LightBase
{
protected:
	DirectX::XMFLOAT3 mPos = { 0.0f, 0.0f, 0.0f };	//位置
	DirectX::XMFLOAT3 mDir = { 0.0f, 1.0f, 0.0f };	//方向
	DirectX::XMFLOAT4 mAmbient = { 1.0f, 1.0f, 1.0f, 1.0f }; //環境光
	DirectX::XMFLOAT4 mDiffuse = { 0.8f, 0.8f, 0.8f, 1.0f }; //拡散反射

	bool isEnable = true;

public:

	LightBase();
	virtual ~LightBase() = default;

	virtual void Update(float dt) = 0;

	DirectX::XMFLOAT3 GetPosition() const  { return mPos; };	//位置
	DirectX::XMFLOAT3 GetDirection() const { return mDir; };	//方向
	DirectX::XMFLOAT4 GetAmbient() const { return mAmbient; }; //環境光
	DirectX::XMFLOAT4 GetDiffuse() const { return mDiffuse; }; //拡散反射

	bool GetEnable() { return isEnable; };

	void SetPosition(const DirectX::XMFLOAT3& _pos);	//位置
	void SetPosition(float* _pos);

	void SetDirection(const DirectX::XMFLOAT3& _dir);	//方向
	void SetDirection(float* _dir);

	void SetAmbient(const DirectX::XMFLOAT4& _ambient); //環境光
	void SetAmbient(float* ambient);
	void SetDiffuse(const DirectX::XMFLOAT4& _diffuse); //拡散反射
	void SetDiffuse(float* diffuse);

	void SetEnable(bool isEnable) { this->isEnable = isEnable; };
};


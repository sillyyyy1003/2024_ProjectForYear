#pragma once
#include <DirectXCollision.h>

/// @brief ŒõŠî’êƒNƒ‰ƒX
class LightBase
{
protected:
	DirectX::XMFLOAT3 mPos = { 0.0f, 0.0f, 0.0f };	//ˆÊ’u
	DirectX::XMFLOAT3 mDir = { 0.0f, 1.0f, 0.0f };	//•ûŒü
	DirectX::XMFLOAT4 mAmbient = { 1.0f, 1.0f, 1.0f, 1.0f }; //ŠÂ‹«Œõ
	DirectX::XMFLOAT4 mDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f }; //ŠgŽU”½ŽË

	float isEnable = true;

public:
	LightBase();
	virtual ~LightBase() = default;

	virtual void Update(float dt) = 0;

	const DirectX::XMFLOAT3& GetPos() { return mPos; };	//ˆÊ’u
	const DirectX::XMFLOAT3& GetDir() { return mDir; };	//•ûŒü
	const DirectX::XMFLOAT4& GetAmbient() { return mAmbient; }; //ŠÂ‹«Œõ
	const DirectX::XMFLOAT4& GetDiffuse() { return mDiffuse; }; //ŠgŽU”½ŽË

	const bool GetEnable() { return isEnable; };

	void SetPos(DirectX::XMFLOAT3 _pos);	//ˆÊ’u
	void SetDir(DirectX::XMFLOAT3 _dir);	//•ûŒü
	void SetAmbient(DirectX::XMFLOAT4 _ambient); //ŠÂ‹«Œõ
	void SetDiffuse(DirectX::XMFLOAT4 _diffuse); //ŠgŽU”½ŽË

	void SetEnable(bool isEnable) { this->isEnable = isEnable; };
};


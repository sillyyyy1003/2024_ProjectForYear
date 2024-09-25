#pragma once
#include <DirectXCollision.h>

/// @brief ŒõŠî’êƒNƒ‰ƒX
class LightBase
{
protected:
	DirectX::XMFLOAT3 mPos = { 0.0f, 0.0f, 0.0f };	//ˆÊ’u
	DirectX::XMFLOAT3 mDir = { 0.0f, 1.0f, 0.0f };	//•ûŒü
	DirectX::XMFLOAT4 mAmbient = { 1.0f, 1.0f, 1.0f, 1.0f }; //ŠÂ‹«Œõ
	DirectX::XMFLOAT4 mDiffuse = { 0.8f, 0.8f, 0.8f, 1.0f }; //ŠgŽU”½ŽË

	bool isEnable = true;

public:

	LightBase();
	virtual ~LightBase() = default;

	virtual void Update(float dt) = 0;

	DirectX::XMFLOAT3 GetPos() const { return mPos; };	//ˆÊ’u
	DirectX::XMFLOAT3 GetDir() const { return mDir; };	//•ûŒü
	DirectX::XMFLOAT4 GetAmbient() const { return mAmbient; }; //ŠÂ‹«Œõ
	DirectX::XMFLOAT4 GetDiffuse() const { return mDiffuse; }; //ŠgŽU”½ŽË

	bool GetEnable() { return isEnable; };

	void SetPos(const DirectX::XMFLOAT3& _pos);	//ˆÊ’u
	void SetPos(float* _pos);

	void SetDir(const DirectX::XMFLOAT3& _dir);	//•ûŒü
	void SetDir(float* _dir);

	void SetAmbient(const DirectX::XMFLOAT4& _ambient); //ŠÂ‹«Œõ
	void SetAmbient(float* ambient);
	void SetDiffuse(const DirectX::XMFLOAT4& _diffuse); //ŠgŽU”½ŽË
	void SetDiffuse(float* diffuse);

	void SetEnable(bool isEnable) { this->isEnable = isEnable; };
};


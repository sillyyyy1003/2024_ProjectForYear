#pragma once
#include <DirectXCollision.h>

/// @brief �����N���X
class LightBase
{
protected:
	DirectX::XMFLOAT3 mPos = { 0.0f, 0.0f, 0.0f };	//�ʒu
	DirectX::XMFLOAT3 mDir = { 0.0f, 1.0f, 0.0f };	//����
	DirectX::XMFLOAT4 mAmbient = { 1.0f, 1.0f, 1.0f, 1.0f }; //����
	DirectX::XMFLOAT4 mDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f }; //�g�U����

	float isEnable = true;

public:
	LightBase();
	virtual ~LightBase() = default;

	virtual void Update(float dt) = 0;

	const DirectX::XMFLOAT3& GetPos() { return mPos; };	//�ʒu
	const DirectX::XMFLOAT3& GetDir() { return mDir; };	//����
	const DirectX::XMFLOAT4& GetAmbient() { return mAmbient; }; //����
	const DirectX::XMFLOAT4& GetDiffuse() { return mDiffuse; }; //�g�U����

	const bool GetEnable() { return isEnable; };

	void SetPos(DirectX::XMFLOAT3 _pos);	//�ʒu
	void SetDir(DirectX::XMFLOAT3 _dir);	//����
	void SetAmbient(DirectX::XMFLOAT4 _ambient); //����
	void SetDiffuse(DirectX::XMFLOAT4 _diffuse); //�g�U����

	void SetEnable(bool isEnable) { this->isEnable = isEnable; };
};


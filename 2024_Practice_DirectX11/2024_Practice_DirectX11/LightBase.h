#pragma once
#include <DirectXCollision.h>

/// @brief �����N���X
class LightBase
{
protected:
	DirectX::XMFLOAT3 mPos = { 0.0f, 0.0f, 0.0f };	//�ʒu
	DirectX::XMFLOAT3 mDir = { 0.0f, 1.0f, 0.0f };	//����
	DirectX::XMFLOAT4 mAmbient = { 1.0f, 1.0f, 1.0f, 1.0f }; //����
	DirectX::XMFLOAT4 mDiffuse = { 0.8f, 0.8f, 0.8f, 1.0f }; //�g�U����

	bool isEnable = true;

public:

	LightBase();
	virtual ~LightBase() = default;

	virtual void Update(float dt) = 0;

	DirectX::XMFLOAT3 GetPos() const { return mPos; };	//�ʒu
	DirectX::XMFLOAT3 GetDir() const { return mDir; };	//����
	DirectX::XMFLOAT4 GetAmbient() const { return mAmbient; }; //����
	DirectX::XMFLOAT4 GetDiffuse() const { return mDiffuse; }; //�g�U����

	bool GetEnable() { return isEnable; };

	void SetPos(const DirectX::XMFLOAT3& _pos);	//�ʒu
	void SetPos(float* _pos);

	void SetDir(const DirectX::XMFLOAT3& _dir);	//����
	void SetDir(float* _dir);

	void SetAmbient(const DirectX::XMFLOAT4& _ambient); //����
	void SetAmbient(float* ambient);
	void SetDiffuse(const DirectX::XMFLOAT4& _diffuse); //�g�U����
	void SetDiffuse(float* diffuse);

	void SetEnable(bool isEnable) { this->isEnable = isEnable; };
};


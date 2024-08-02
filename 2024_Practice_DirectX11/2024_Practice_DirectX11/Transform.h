#pragma once
#include "D3DUtil.h"

///�ʒu�A��]�A�傫���Ȃǂ̊Ǘ�
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

	/// @brief �f�[�^���s��ɕϊ�
	/// @return �s���Ԃ�
	DirectX::XMFLOAT4X4 GetMatrix();

	void Update();

private:
	/// @brief Transform�f�[�^���s��ɕϊ�����
	void GenerateWorld();
};


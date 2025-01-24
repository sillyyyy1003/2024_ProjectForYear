#pragma once
#include "ParticleEffect.h"

class Potion;
class PotionEffect :public ParticleEffect
{
	Potion* mPotion = nullptr;
	DirectX::XMFLOAT3 mCenter = {};
	float mRadius = 0.0f;

	//�`�悷�邽��PixelShader
	PixelShader* mPixelShader = nullptr;
	Texture* mParticleTex=nullptr;

public:
	/// @brief 
	/// @param filePath 
	/// @param particleNum 
	/// @param size 
	void InitParticleRenderer(const char* filePath, uint32_t particleNum, float size = 0.01f) override;

	//�|�[�V�����̐F���l�����邽�߁@�|�C���^�[���擾
	void SetPotion(Potion* potion);

	//���q�����X�V
	void UpdateParticle() override;

	void UpdateParticleState(ParticleData& data, float dt) override;

	void InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity) override;

	void Draw() override;
};


#pragma once
#include "ParticleEffect.h"

class Potion;
class PotionEffect :public ParticleEffect
{
	Potion* mPotion = nullptr;
	DirectX::XMFLOAT3 mCenter = {};
	float mRadius = 0.0f;

	//描画するためPixelShader
	PixelShader* mPixelShader = nullptr;
	Texture* mParticleTex=nullptr;

public:
	/// @brief 
	/// @param filePath 
	/// @param particleNum 
	/// @param size 
	void InitParticleRenderer(const char* filePath, uint32_t particleNum, float size = 0.01f) override;

	//ポーションの色を獲得するため　ポインターを取得
	void SetPotion(Potion* potion);

	//粒子情報を更新
	void UpdateParticle() override;

	void UpdateParticleState(ParticleData& data, float dt) override;

	void InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity) override;

	void Draw() override;
};


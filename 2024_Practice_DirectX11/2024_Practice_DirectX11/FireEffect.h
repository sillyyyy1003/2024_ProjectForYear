#pragma once
#include "ParticleEffect.h"

class FireEffect : public ParticleEffect
{
private:
	//•`‰æ‚·‚é‚½‚ßPixelShader
	PixelShader* mPixelShader = nullptr;
	Texture* mParticleTex = nullptr;

	DirectX::XMFLOAT3 mFireCenter = {};
	float mFireRadius = 0.0f;

public:
	void InitParticleRenderer(const char* filePath, uint32_t particleNum, float size = 0.01f) override;

	void InitFireParticleData(const DirectX::XMFLOAT3& center,const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity);
	void InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity) override;

	void UpdateParticleState(ParticleData& data, float dt) override;

	void Draw() override;
};


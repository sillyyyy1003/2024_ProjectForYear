#pragma once
#include "ParticleEffect.h"
namespace Fire
{
	constexpr float CANDLE_PARTICLE_GENERATE_THETA = 360.f;
	constexpr float CANDLE_PARTICLE_GENERATE_PHI = 160.f;

}

class FireEffect : public ParticleEffect
{
private:

	//•`‰æ‚·‚é‚½‚ßPixelShader
	PixelShader* mPixelShader = nullptr;
	Texture* mParticleTex = nullptr;

	DirectX::XMFLOAT3 mFireOffset = {};

	DirectX::XMFLOAT4 mStartColor = {};
	DirectX::XMFLOAT4 mEndColor = {};

public:
	void InitParticleRenderer(uint32_t particleNum, float size = 0.01f);

	void InitFireParticleData(const DirectX::XMFLOAT3& emitPos,const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity,float lifeTime);
	void InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity) override;

	std::vector<ParticleMesh::ParticleInstanceData> UpdateInstanceData(float dt) override;
	void UpdateParticleState(ParticleData& data, float dt) override;
	void UpdateEmitPos(DirectX::XMFLOAT3 emitPos);

	void Draw() override;

	void SetParticleColorRange(DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor) { mStartColor = startColor; mEndColor = endColor; };

	const DirectX::XMFLOAT4& GetStartColor() const { return mStartColor; };
	const DirectX::XMFLOAT4& GetEndColor() const { return mEndColor; };
};


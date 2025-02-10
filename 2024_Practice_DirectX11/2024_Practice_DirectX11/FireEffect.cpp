#include "FireEffect.h"
#include "GameApp.h"
#include "MovableStaticObject.h"
#include "RandomGenerator.h"

using namespace DirectX;

void FireEffect::InitParticleRenderer(uint32_t particleNum, float size)
{
	ParticleEffect::InitParticleRenderer("VS_ParticleInstance.hlsl", particleNum, size);

	//Pixel Shaderの初期化
	mPixelShader = SceneManager::Get()->GetObj<PixelShader>("PS_ParticleInstance").get();
	mParticleTex = SceneManager::Get()->GetObj<Texture>("PotionParticle").get();
	
}

void FireEffect::InitFireParticleData(const DirectX::XMFLOAT3& emitPos, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity, float lifeTime)
{
	//粒子存続時間
	SetParticleLifeTime(lifeTime);

	//基準値を設定する
	mEmitPos = emitPos;			// 发射位置
	mEmitVel = velocity;      // 发射速度
	mEmitAccel = emitAccel;   // 发射加速度

	for (auto& particle : mParticleData)
	{
		particle.accumulateTime = 0.0f;
		particle.initPos = mEmitPos;
		particle.pos = particle.initPos;
		particle.intiVel = mEmitVel * RandomGenerator::Get()->RandomDirection(Fire::CANDLE_PARTICLE_GENERATE_THETA, Fire::CANDLE_PARTICLE_GENERATE_PHI); //ランダム上方向
		particle.acceleration = mEmitAccel * RandomGenerator::Get()->RandomFloat(); //ランダム加速度
		particle.lifeTime = mParticleLifeTime * RandomGenerator::Get()->RandomFloat();  //ランダム存続時間
		particle.accumulateTime = 0.0f;
		particle.color = mStartColor; // リセット色
	}

}

void FireEffect::InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity)
{
	ParticleEffect::InitPointOnCircleParticle(center, radius, emitAccel, velocity);
}

std::vector<ParticleMesh::ParticleInstanceData> FireEffect::UpdateInstanceData(float dt)
{
	std::vector<ParticleMesh::ParticleInstanceData> instanceData;
	instanceData.resize(mParticleNum);
	for (int i = 0; i < mParticleData.size(); i++)
	{
		//粒子状態をアップデート
		UpdateParticleState(mParticleData[i], dt);
		Vector3 scale = mScale * (1.f - mParticleData[i].accumulateTime / mParticleData[i].lifeTime);
		XMMATRIX scaleMatrix = XMMatrixScaling(scale.x,scale.y,scale.z);
		XMStoreFloat4x4(&instanceData[i].WorldMatrix, XMMatrixTranspose(scaleMatrix * XMMatrixIdentity() * XMMatrixTranslation(mParticleData[i].pos.x, mParticleData[i].pos.y, mParticleData[i].pos.z)));

		UpdateParticleColor(instanceData[i], mParticleData[i]);

	}
	return instanceData;
}

void FireEffect::UpdateParticleState(ParticleData& data, float dt)
{

	data.accumulateTime += dt;

	if (data.accumulateTime >= data.lifeTime)
	{
		// 重置粒子状态
		data.accumulateTime = 0.0f;
		data.initPos = mEmitPos;
		data.pos = data.initPos;
		data.intiVel = mEmitVel * RandomGenerator::Get()->RandomDirection(Fire::CANDLE_PARTICLE_GENERATE_THETA, Fire::CANDLE_PARTICLE_GENERATE_PHI); //ランダム上方向
		data.acceleration = mEmitAccel * RandomGenerator::Get()->RandomFloat(); //ランダム加速度
		data.lifeTime = mParticleLifeTime * RandomGenerator::Get()->RandomFloat();  //ランダム存続時間
		data.accumulateTime = 0.0f;
		data.color = mStartColor; // リセット色
	}
	else
	{
		// 更新粒子位置
		float t = data.accumulateTime;
		data.pos = data.initPos + data.intiVel * t + 0.5f * data.acceleration * t * t;

		//色の変化
		float colorT = data.accumulateTime / data.lifeTime;
		data.color = LerpColor(mStartColor, mEndColor, colorT);

	}

}

void FireEffect::UpdateEmitPos(DirectX::XMFLOAT3 emitPos)
{
	mEmitPos = emitPos;
}

void FireEffect::Draw()
{
	if (!isAlive)return;

	ID3D11DeviceContext* pContext = gD3D->GetContext();

	//UpdateConstant Buffer
	CameraBase* firstCamera = GameApp::GetCurrentCamera();
	ConstantBuffer cBuffer;
	cBuffer.view = firstCamera->GetViewXMF();
	cBuffer.proj = firstCamera->GetProjXMF();
	pContext->UpdateSubresource(mBuffer.Get(), 0, nullptr, &cBuffer, 0, 0);

	//Set Constant Buffer to vertexShader
	pContext->VSSetConstantBuffers(0, 1, mBuffer.GetAddressOf());
	pContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	pContext->IASetInputLayout(mInputLayout.Get());

	//Set PixelShader
	mPixelShader->SetShader();
	mPixelShader->SetTexture(0, mParticleTex);
	mParticleMesh->DrawInstance();
}


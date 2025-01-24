#include "FireEffect.h"
#include "GameApp.h"
#include "RandomGenerator.h"


void FireEffect::InitParticleRenderer(const char* filePath, uint32_t particleNum, float size)
{
	ParticleEffect::InitParticleRenderer(filePath, particleNum, size);

	//Pixel Shader‚Ì‰Šú‰»
	mPixelShader = SceneManager::Get()->GetObj<PixelShader>("PS_ParticleInstance").get();
	mParticleTex = SceneManager::Get()->GetObj<Texture>("FireParticle").get();
}

void FireEffect::InitFireParticleData(const DirectX::XMFLOAT3& center,const DirectX::XMFLOAT3& emitAccel,
	const DirectX::XMFLOAT3& velocity)
{
	mEmitPos = center;
	mEmitVel = velocity;
	mEmitAccel = emitAccel;

	for (auto& i : mParticleData)
	{
		i.initPos = mEmitPos;
		i.intiVel = mEmitVel * RandomGenerator::Get()->RandomDirection(45.f,45.f);
		i.acceleration = mEmitAccel * RandomGenerator::Get()->RandomFloat();
	}
}

void FireEffect::InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity)
{
	ParticleEffect::InitPointOnCircleParticle(center, radius, emitAccel, velocity);
}

void FireEffect::UpdateParticleState(ParticleData& data, float dt)
{
	data.accumulateTime += dt;
	if (data.accumulateTime >= data.lifeTime)//—±Žq‚ÌŽõ–½ŽžŠÔ‚É’´‚¦‚½‚ç
	{
		//reset time
		data.accumulateTime = 0.0f;
		//reset position
		data.initPos = mEmitPos;
		data.pos = data.initPos;
		data.intiVel = mEmitVel * RandomGenerator::Get()->RandomDirection(45.f, 45.f);
		data.acceleration = mEmitAccel * RandomGenerator::Get()->RandomFloat();
		//reset lifetime
		data.lifeTime = mParticleLifeTime * RandomGenerator::Get()->RandomFloat();

	}
	else
	{
		data.pos = 0.5f * data.accumulateTime * data.accumulateTime * data.acceleration + data.initPos + data.intiVel * data.accumulateTime;

	}

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

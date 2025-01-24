#include "PotionEffect.h"

#include "GameApp.h"
#include "MovableStaticObject.h"
#include "RandomGenerator.h"
#include "Potion.h"

void PotionEffect::InitParticleRenderer(const char* filePath, uint32_t particleNum, float size)
{
	ParticleEffect::InitParticleRenderer(filePath, particleNum, size);

	//Pixel Shaderの初期化
	mPixelShader = SceneManager::Get()->GetObj<PixelShader>("PS_ParticleInstance").get();
	mParticleTex = SceneManager::Get()->GetObj<Texture>("PotionParticle").get();
}

void PotionEffect::SetPotion(Potion* potion)
{
	mPotion = potion;
}

void PotionEffect::UpdateParticle()
{
	//ポーションから色取得
	Color color = mPotion->GetPotionColor();
	
	for (auto& i : mParticleData)
	{
		//粒子色を更新する
		i.color = RandomGenerator::Get()->RandomColor(color, 10.f);

		//粒子
		mCenter = mPotion->GetPosition();

	}

}

void PotionEffect::UpdateParticleState(ParticleData& data, float dt)
{
	data.accumulateTime += dt;
	if (data.accumulateTime >= data.lifeTime)//粒子の寿命時間に超えたら
	{
		//reset time
		data.accumulateTime = 0.0f;
		//reset position
		data.initPos = RandomGenerator::Get()->RandomPointOnCircle(mRadius, mCenter); 
		data.pos = data.initPos;
		//reset velocity
		data.intiVel = mEmitVel * RandomGenerator::Get()->RandomFloat();
		//reset acceleration
		data.acceleration = mEmitAccel * RandomGenerator::Get()->RandomFloat();
		//reset lifetime
		data.lifeTime = mParticleLifeTime * RandomGenerator::Get()->RandomFloat();

	}
	else
	{
		data.pos = 0.5f * data.accumulateTime * data.accumulateTime * data.acceleration + data.initPos + data.intiVel * data.accumulateTime;
		
	}

}

void PotionEffect::InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius,
	const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity)
{
	ParticleEffect::InitPointOnCircleParticle(center, radius, emitAccel, velocity);
	mCenter = center;
	mRadius = radius;
}

void PotionEffect::Draw()
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

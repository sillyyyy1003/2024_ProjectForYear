#define _CRT_SECURE_NO_WARNINGS
#include "ParticleEffect.h"
#include <iostream>
#include "GameApp.h"
#include "MovableStaticObject.h"
#include "RandomGenerator.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;



void ParticleEffect::InitParticleRenderer(const char* filePath, uint32_t particleNum, float size)
{
	mParticleNum = particleNum;
	mParticleSize = size;

	DirectX::XMFLOAT3 pos[] = {

		XMFLOAT3(-mParticleSize/2.f, -mParticleSize / 2.f,0 ),
		XMFLOAT3(-mParticleSize / 2.f, mParticleSize / 2.f,0),
		XMFLOAT3(mParticleSize / 2.f, mParticleSize / 2.f, 0),
		XMFLOAT3(mParticleSize / 2.f, -mParticleSize / 2.f,0),

	};

	//todo:if use tangent need to fix the vertex
	std::vector<Vertex::VtxPosNormalTex> vtx;
	vtx.resize(4);

	vtx = {

		{pos[0],Vector3(0.0f,0.0f,1.0f),Vector2(0.f,1.0f)},
		{pos[1],Vector3(0.0f,0.0f,1.0f),Vector2(0.f,0.f)},
		{pos[2],Vector3(0.0f,0.0f,1.0f),Vector2(1.0f,0.f)},
		{pos[3],Vector3(0.0f,0.0f,1.0f),Vector2(1.0f ,1.0f)},
	};

	std::vector<DWORD> indexData;

	indexData.resize(6);

	indexData = {
		0,1,2,2,3,0
	};

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = indexData.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(indexData.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mParticleMesh = std::make_unique<ParticleMesh>(desc, mParticleNum);

	//データを更新する
	std::vector<ParticleMesh::ParticleInstanceData> instanceData;
	instanceData.resize(mParticleNum);
	mParticleMesh->WriteInstanceData(instanceData.data());

	//頂点の初期化を行う
	mParticleData.resize(mParticleNum);

	//Create Vertex Shader
	CreateVertexShader(filePath, "main");

	//InputLayoutの作成
	D3D11_INPUT_ELEMENT_DESC elem[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},


		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{
			"INSTANCE_MTX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_INSTANCE_DATA, 1
			},
		{
			"INSTANCE_MTX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_INSTANCE_DATA, 1
			},
		{
			"INSTANCE_MTX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_INSTANCE_DATA, 1
			},
		{
			"INSTANCE_MTX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_INSTANCE_DATA, 1
			},
	
	};
	CreateInputLayout(elem, 8, filePath, "main");
	CreateConstantBuffer(sizeof(ParticleEffect::ConstantBuffer), mBuffer.GetAddressOf());

}

void ParticleEffect::InitParticleData(DirectX::XMFLOAT3 emitPos, DirectX::XMFLOAT3 emitAccel, DirectX::XMFLOAT3 velocity)
{
	mEmitPos = emitPos;
	mEmitVel = velocity;
	mEmitAccel = emitAccel;

	for(auto& i:mParticleData)
	{
		i.initPos = mEmitPos;
		i.acceleration = emitAccel;
		i.intiVel = mEmitVel;
	}
}

void ParticleEffect::InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel,const DirectX::XMFLOAT3& velocity)
{
	mEmitVel = velocity;
	mEmitAccel = emitAccel;

	for (auto& i : mParticleData)
	{
		i.initPos = RandomGenerator::Get()->RandomPointOnCircle(radius, center);
		i.acceleration = mEmitAccel*RandomGenerator::Get()->RandomFloat();
		i.intiVel = mEmitVel* RandomGenerator::Get()->RandomFloat();
	}
}

void ParticleEffect::InitParticleColor(Color color)
{
	for (auto& i : mParticleData)
	{
		i.color = color;
	}
}

void ParticleEffect::InitRandomParticleColor()
{
	for (auto& i : mParticleData)
	{
		i.color = RandomGenerator::Get()->RandomColor();
	}
}

void ParticleEffect::InitRandomParticleColor(const DirectX::XMFLOAT2& colorRange, const DirectX::XMFLOAT2& valueRange, const DirectX::XMFLOAT2& saturationRange)
{
	for (auto& i : mParticleData)
	{
		i.color = RandomGenerator::Get()->RandomColor(valueRange,saturationRange,colorRange);
	}
}

void ParticleEffect::InitRandomParticleColor(float color, float range, const DirectX::XMFLOAT2& valueRange,
	const DirectX::XMFLOAT2& saturationRange)
{
	for (auto& i : mParticleData)
	{
		i.color = RandomGenerator::Get()->RandomColor(valueRange, saturationRange, { color-range,color + range });
	}
}

void ParticleEffect::Update(float dt)
{
	//作動しない場合
	if (!isAlive)return;

	if(!isLoop)
	{
		if (mTime < mEffectAliveTime)
		{
			mTime += dt;
		}
		else
		{
			//reset time
			mTime = 0;
			isAlive = false;

			//reset all the particle
			ResetParticle();
		}
	}

	UpdateParticle();

	mParticleMesh->WriteInstanceData(UpdateInstanceData(dt).data());
}

void ParticleEffect::Draw()
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


	mParticleMesh->DrawInstance();
}



void ParticleEffect::CreateVertexShader(const std::string& filename, const std::string& entryPath)
{
#if defined(_DEBUG)
	// グラフィックデバッグツールによるシェーダーのデバッグを有効にする
	UINT	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT	compileFlags = 0;
#endif
	ComPtr<ID3DBlob> blob;
	wchar_t ws[512];

	setlocale(LC_CTYPE, "jpn");
	mbstowcs(ws, filename.c_str(), 512);
	ComPtr<ID3DBlob> pErrorBlob = NULL;
	HR(D3DCompileFromFile(ws, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPath.c_str(), "vs_5_0", compileFlags, 0, &blob, &pErrorBlob));
	HR(gD3D->GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, mVertexShader.GetAddressOf()));
}

void ParticleEffect::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT elem_num, const std::string& filename,
	const std::string& entryPath)
{
#if defined(_DEBUG)
	// グラフィックデバッグツールによるシェーダーのデバッグを有効にする
	UINT	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT	compileFlags = 0;
#endif
	ComPtr<ID3DBlob> blob;
	wchar_t ws[512];

	setlocale(LC_CTYPE, "jpn");
	mbstowcs(ws, filename.c_str(), 512);
	ComPtr<ID3DBlob> pErrorBlob = NULL;
	HR(D3DCompileFromFile(ws, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPath.c_str(), "vs_5_0", compileFlags, 0, &blob, &pErrorBlob));
	HR(gD3D->GetDevice()->CreateInputLayout(layout, elem_num, blob->GetBufferPointer(),
		blob->GetBufferSize(), mInputLayout.GetAddressOf()));

}

void ParticleEffect::CreateConstantBuffer(unsigned int byteSize, ID3D11Buffer** CBuffer)
{// コンスタントバッファ生成
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR(gD3D->GetDevice()->CreateBuffer(&bd, nullptr, CBuffer));
}

std::vector<ParticleMesh::ParticleInstanceData> ParticleEffect::UpdateInstanceData(float dt)
{
	std::vector<ParticleMesh::ParticleInstanceData> instanceData;
	instanceData.resize(mParticleNum);
	for (int i = 0; i < mParticleData.size(); i++)
	{
		//粒子状態をアップデート
		UpdateParticleState(mParticleData[i], dt);
	
		XMStoreFloat4x4(&instanceData[i].WorldMatrix, XMMatrixTranspose(XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(mParticleData[i].pos.x, mParticleData[i].pos.y, mParticleData[i].pos.z)));

		UpdateParticleColor(instanceData[i],mParticleData[i]);

	}
	return instanceData;
}

void ParticleEffect::UpdateParticleState(ParticleData& data,float dt)
{
	data.accumulateTime += dt;
	if (data.accumulateTime >= data.lifeTime)//粒子の寿命時間に超えたら
	{
		//reset time
		data.accumulateTime = 0.0f;
		//reset position
		data.pos = data.initPos;
		//reset velocity
		data.intiVel= mEmitVel*RandomGenerator::Get()->RandomFloat();
		//reset acceleration
		data.acceleration=mEmitAccel * RandomGenerator::Get()->RandomFloat();
		//reset lifetime
		data.lifeTime = mParticleLifeTime * RandomGenerator::Get()->RandomFloat();
	
	}else
	{
		data.pos = 0.5f * data.accumulateTime * data.accumulateTime * data. acceleration + data.initPos + data.intiVel * data.accumulateTime;
		
	}


}

void ParticleEffect::UpdateParticleColor(ParticleMesh::ParticleInstanceData& instanceData, ParticleData& data)
{
	instanceData.Color = { data.color.x,data.color.y,data.color.z ,data.color.w * (1.f - data.accumulateTime / data.lifeTime) };
}



void ParticleEffect::ResetParticle()
{
	for(auto& i:mParticleData)
	{
		i.accumulateTime = 0.0f;
		i.pos = i.initPos;
	}
}

void ParticleEffect::SetParticleAliveTime(float time)
{
	mParticleLifeTime = time;
	for (auto& i : mParticleData)
	{
		i.lifeTime = mParticleLifeTime * RandomGenerator::Get()->RandomFloat(0.1f, 1.0f);
	}
}

void ParticleEffect::SetParticleLifeTime(float lifeTime)
{
	mParticleLifeTime = lifeTime;
}


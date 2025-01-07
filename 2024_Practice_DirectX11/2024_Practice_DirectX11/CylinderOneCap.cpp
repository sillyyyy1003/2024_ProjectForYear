﻿#include "CylinderOneCap.h"

#include "CylinderOneCap.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

CylinderOneCap::CylinderOneCap() :Primitive(PrimitiveConfig::CYLINDER_ONECAP)
{
}

void CylinderOneCap::Init(const char* filePath, int slices, int stacks, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);


	CreateMesh(slices, stacks);
	CreateMaterial();
	CreateTexture(filePath);

}

void CylinderOneCap::Init(const std::shared_ptr<Texture>& tex, int slices, int stacks, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh(slices, stacks);
	CreateMaterial();
	LoadTexture(tex);

}

void CylinderOneCap::Update(float dt)
{
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}


void CylinderOneCap::Draw(int texSlot)
{
	SetDefShader();

	mPS->SetShader();
	mVS->SetShader();

	if (texSlot >= 0)
		mPS->SetTexture(texSlot, mMaterial.tex.get());

	mMesh->Draw();
}

void CylinderOneCap::CreateMesh(UINT slices, UINT stacks)
{
	std::vector<Vertex::VtxPosNormalTex> vtx;
	std::vector<DWORD> idx;

	float height = 1.f;
	float radius = 0.5f;

	float h2 = height / 2;
	float theta = 0.0f;
	float per_theta = XM_2PI / slices;
	float stackHeight = height / (float)stacks;

	// Generate vertices
	//body
	for (UINT i = 0; i <= stacks; i++)
	{
		float y = -h2 + i * stackHeight;
		for (UINT j = 0; j <= slices; j++)
		{
			theta = j * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);

			float u = theta / (XM_2PI * mUvAnimation->GetSplit().x);			float v = 1.0f - i / (stacks * mUvAnimation->GetSplit().y);

			vtx.push_back({
				Vector3(x, y, z),
				Vector3(cosf(theta), 0.0f, sinf(theta)),
				Vector2(u,v)
				});
		}
	}

	//bot center
	vtx.push_back({
		Vector3(0.0f, -h2, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f),
		XMFLOAT2(0.5f, 0.5f)
		});

	//bot cap
	for (UINT i = 0; i <= slices; i++)
	{
		theta = i * per_theta;
		float u = cosf(theta) * radius / (height*mUvAnimation->GetSplit().x) + 0.5f;
		float v = sinf(theta) * radius / (height * mUvAnimation->GetSplit().x) + 0.5f;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		vtx.push_back({
			Vector3(x, -h2, z),
			Vector3(0.0f, -1.0f, 0.0f),
			XMFLOAT2(u,v)
			});
	}

	for (UINT i = 0; i < stacks; i++)
	{
		for (UINT j = 0; j < slices; j++)
		{
			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);

			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);
			idx.push_back(i * (slices + 1) + j + 1);
		}
	}


	// Top cap indices
	UINT offset = (slices + 1) * (stacks + 1);
	for (UINT i = 1; i <= slices; i++)
	{
		idx.push_back(offset);
		idx.push_back(offset + i);
		idx.push_back(offset + i % (slices + 1) + 1);
		
	}


	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mMesh = std::make_unique<Mesh>(desc);

	//頂点情報をmVerticesに渡す
	SetVertices(vtx);
}


void CylinderOneCap::WriteDefShader()
{
	if (!mDefPS || !mDefVS)
	{
		DebugLog::LogError("ShaderFile is not set");
		return;
	}

	CameraBase* firstCamera = GameApp::GetCurrentCamera();
	std::shared_ptr<DirLight> dirLight = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight");
	XMFLOAT4X4 WVP[3] = {};
	//WORLD
	WVP[0] = mTransform.GetMatrixFX4();

	//VIEW
	WVP[1] = firstCamera->GetViewXMF();

	//PROJ
	WVP[2] = firstCamera->GetProjXMF();

	XMFLOAT4 eyePos = { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

	NormalConstantBuffer cb = {


			eyePos,
			Vector4{dirLight->GetAmbient().x,dirLight->GetAmbient().y,dirLight->GetAmbient().z,dirLight->GetAmbient().w},
		   Vector4{ dirLight->GetDiffuse().x,dirLight->GetDiffuse().y,dirLight->GetDiffuse().z,dirLight->GetDiffuse().w},
		Vector4{dirLight->GetPosition().x,dirLight->GetPosition().y,dirLight->GetPosition().z,0},
		mMaterial.material,
	};


	UVConstantBuffer uvBuffer;
	uvBuffer.useUV = isUseUVAnimation;
	//UV MATRIX 作成
	if (isUseUVAnimation)
	{
		uvBuffer.uv = XMMatrixTranslation(mUvAnimation->GetOffsetUV().x, mUvAnimation->GetOffsetUV().y, 0.0f);
		uvBuffer.uv = XMMatrixTranspose(uvBuffer.uv);
	}

	mDefVS->WriteShader(0, WVP);
	mDefVS->WriteShader(1, &uvBuffer);
	mDefPS->WriteShader(0, &cb);
}

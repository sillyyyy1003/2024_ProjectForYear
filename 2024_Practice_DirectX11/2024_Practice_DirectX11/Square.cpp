#include "Square.h"
#include "D3DUtil.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include <functional>

using namespace DirectX;
using namespace SimpleMath;


Square::Square() :Primitive(PrimitiveConfig::SQUARE)
{

}

void Square::SetScale(const DirectX::XMFLOAT2& scale)  noexcept
{
	mTransform.SetScale(scale.x, 1.0f, scale.y);
}


void Square::Init(const char* _fileName, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh();
	CreateMaterial();
	CreateTexture(_fileName);
}

void Square::Init(const std::shared_ptr<Texture>& tex, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh();
	CreateMaterial();
	LoadTexture(tex);
}


void Square::Init(const char* _fileName, int slices,  DirectX::XMINT2 _UVSplit) 
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	if (slices)
		CreateMesh(slices);
	else
		CreateMesh();

	CreateMaterial();
	CreateTexture(_fileName);
}

void Square::Init(const std::shared_ptr<Texture>& tex, int slices,DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	if (slices)
		CreateMesh(slices);
	else
		CreateMesh();

	CreateMaterial();
	LoadTexture(tex);
}

void Square::Update(float dt)
{
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}

void Square::Draw(int texSlot)
{
	SetDefShader();

	mPS->SetShader();
	mVS->SetShader();
	if (texSlot >= 0)
		mPS->SetTexture(texSlot, mMaterial.tex.get());
	mMesh->Draw();
}


void Square::CreateMesh()
{
	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 pos[] = {

		XMFLOAT3(-d, 0, -d),
		XMFLOAT3(-d, 0, d),
		XMFLOAT3(d, 0, d),
		XMFLOAT3(d, 0, -d),

	};

	//todo:if use tangent need to fix the vertex
	std::vector<Vertex::VtxPosNormalTex> vtx;
	vtx.resize(8);

	vtx = {

		{pos[0],Vector3(0.0f,1.0f,0.0f),Vector2(0.f,1.0f / mUvAnimation->GetSplit().y)},
		{pos[1],Vector3(0.0f,1.0f,0.0f),Vector2(0.f,0.f)},
		{pos[2],Vector3(0.0f,1.0f,0.0f),Vector2(1.0f / mUvAnimation->GetSplit().x,0.f)},
		{pos[3],Vector3(0.0f,1.0f,0.0f),Vector2(1.0f / mUvAnimation->GetSplit().x,1.0f / mUvAnimation->GetSplit().y)},

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
	mMesh = std::make_unique<Mesh>(desc);
	//頂点情報をmVerticesに渡す
	SetVertices(vtx);
}


void Square::CreateMesh(UINT slices)
{
	
	
	float size = 1.0f;
	uint32_t vertexCount = (slices + 1) * (slices + 1);
	uint32_t indexCount = 6 * slices * slices;
	std::vector<Vertex::VtxPosNormalTex> vtx;
	std::vector<DWORD> indexData;
	vtx.resize(vertexCount);
	indexData.resize(indexCount);


	float sliceWidth = size / slices;
	float sliceDepth = size / slices;
	float leftBottomX = -size / 2;
	float leftBottomZ = -size / 2;
	float posX, posZ;
	float sliceTexWidth = 1.0f / mUvAnimation->GetSplit().x / slices;
	float sliceTexDepth = 1.0f / mUvAnimation->GetSplit().y / slices;
	float normal;
	uint32_t vIndex = 0;
	uint32_t iIndex = 0;
	//  __ __
	  // | /| /|
	  // |/_|/_|
	  // | /| /| 
	  // |/_|/_|
	for (uint32_t z = 0; z <= slices; ++z)
	{
		posZ = leftBottomZ + z * sliceDepth;
		for (uint32_t x = 0; x <= slices; ++x)
		{
			posX = leftBottomX + x * sliceWidth;
			normal = 1.0f;
			vtx[vIndex]= {
				{posX,0.0f,posZ},
				{0.f,1.f,0.f},
				{x * sliceTexWidth,1.0f / mUvAnimation->GetSplit().y - z * sliceTexDepth}
			};
			vIndex++;
		}
	}

	SetVertices(vtx);

	
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < slices; ++j)
		{
			
				indexData[iIndex++] = i * (slices + 1) + j;
				indexData[iIndex++] = (i + 1) * (slices + 1) + j;
				indexData[iIndex++] = (i + 1) * (slices + 1) + j + 1;

				indexData[iIndex++] = (i + 1) * (slices + 1) + j + 1;
				indexData[iIndex++] = i * (slices + 1) + j + 1;
				indexData[iIndex++] = i * (slices + 1) + j;
		}
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = indexData.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(indexData.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mMesh = std::make_unique<Mesh>(desc);

}

void Square::WriteDefShader()
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



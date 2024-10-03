#include "Plane3D.h"
#include "D3DUtil.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"

using namespace DirectX;
using namespace SimpleMath;


Plane3D::Plane3D() :Primitive(PLANE)
{

}

void Plane3D::SetTexUV(DirectX::XMFLOAT2 _texUV) noexcept
{
	mTexUV = _texUV;
}

void Plane3D::SetScale(const DirectX::XMFLOAT2& scale)  noexcept
{
	mTransform.SetScale(scale.x, 1.0f, scale.y);
}


void Plane3D::Init(const char* _fileName)
{
	CreateMesh();
	CreateMaterial();
	CreateTexture(_fileName);
	LoadDefShader();
}

void Plane3D::Init(const char* _fileName, DirectX::XMFLOAT2 _texUV)
{
	SetTexUV(_texUV);
	Init(_fileName);
}

void Plane3D::Init(const char* _fileName, int slices)
{
	CreateMesh(slices);
	CreateMaterial();
	CreateTexture(_fileName);
}

void Plane3D::Update(float dt)
{
	WriteDefShader();
}

void Plane3D::Draw(int texSlot)
{
	SetDefShader();
	mPS->SetShader();
	mVS->SetShader();
	if (texSlot >= 0)
		mPS->SetTexture(texSlot, mMaterial.tex.get());
	mMesh->Draw();
}


const void Plane3D::CreateMesh()
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

		{pos[0],Vector3(0.0f,1.0f,0.0f),Vector2(0.f,0.f)},
		{pos[1],Vector3(0.0f,1.0f,0.0f),Vector2(mTexUV.x,0.f)},
		{pos[2],Vector3(0.0f,1.0f,0.0f),Vector2(mTexUV.x,mTexUV.y)},
		{pos[3],Vector3(0.0f,1.0f,0.0f),Vector2(0.f,mTexUV.y)},

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

const void Plane3D::CreateMaterial()
{
	mMaterial.material = 
	{
		Color(1.0f, 1.0f, 1.0, 1.0f),		// 環境光
		Color(1.0f, 1.0f, 1.0, 1.0f),		// 表面色
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
	};

}

const void Plane3D::CreateTexture(const char* fileName)
{
	if (!fileName)
	{
		mMaterial.material.isTexEnable = false;
		mMaterial.tex = nullptr;
		return;
	}

	mMaterial.tex = std::make_unique<Texture>();
	HRESULT hr = mMaterial.tex->Create(fileName);
	if (FAILED(hr))
	{
		mMaterial.tex = nullptr;
		mMaterial.material.isTexEnable = false;
	}
	mFilePath = fileName;
}

const void Plane3D::CreateMesh(UINT slices)
{
	float size = 1.0f; 
	float halfSize = size / 2;  
	float step = size / slices; 
	std::vector<Vertex::VtxPosNormalTex> vtx; 

	for (UINT i = 0; i <= slices; i++)
	{
		for (UINT j = 0; j <= slices; j++)
		{
			float x = -halfSize + j * step; // x 
			float z = -halfSize + i * step; // z 
			float u = static_cast<float>(j) / slices; // u
			float v = static_cast<float>(i) / slices; // v

			vtx.push_back({ XMFLOAT3(x, 0, z), Vector3(0.0f, 1.0f, 0.0f), Vector2(u, v) });
		}
	}

	std::vector<DWORD> indexData;
	for (UINT i = 0; i < slices; i++)
	{
		for (UINT j = 0; j < slices; j++)
		{
			int start = i * (slices + 1) + j;
			indexData.push_back(start);
			indexData.push_back(start + 1);
			indexData.push_back(start + slices + 2);

			indexData.push_back(start);
			indexData.push_back(start + slices + 2);
			indexData.push_back(start + slices + 1);
		}
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = indexData.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(indexData.size());
	//desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	mMesh = std::make_unique<Mesh>(desc);
}

void Plane3D::WriteDefShader()
{
	FirstPersonCamera* firstCamera = GameApp::GetComponent<FirstPersonCamera>("Camera");
	DirLight* dirLight = GameApp::GetComponent<DirLight>("Light");

	XMFLOAT4X4 WVP[3] = {};
	//WORLD
	WVP[0] = mTransform.GetMatrixFX4();

	//VIEW
	WVP[1] = firstCamera->GetViewXMF();

	//PROJ
	WVP[2] = firstCamera->GetProjXMF();

	XMFLOAT4 eyePos = { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

	struct Light
	{
		XMFLOAT4 lightAmbient;
		XMFLOAT4 lightDiffuse;
		XMFLOAT4 lightDir;
	};

	Light light = {
		dirLight->GetAmbient(),
		dirLight->GetDiffuse(),
		Vector4{dirLight->GetPos().x,dirLight->GetPos().y,dirLight->GetPos().z,0},
	};

	mDefVS->WriteShader(0, WVP);
	mDefPS->WriteShader(0, &mMaterial.material);
	mDefPS->WriteShader(1, &eyePos);
	mDefPS->WriteShader(2, &light);
}



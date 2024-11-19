#include "Square.h"
#include "D3DUtil.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"

using namespace DirectX;
using namespace SimpleMath;


Square::Square() :Primitive(PLANE)
{

}

void Square::SetTexUV(DirectX::XMFLOAT2 _texUV) noexcept
{
	mTexUV = _texUV;
}

void Square::SetScale(const DirectX::XMFLOAT2& scale)  noexcept
{
	mTransform.SetScale(scale.x, 1.0f, scale.y);
}


void Square::Init(const char* _fileName)
{
	CreateMesh();
	CreateMaterial();
	CreateTexture(_fileName);
	LoadDefShader();
}

void Square::Init(const char* _fileName, DirectX::XMFLOAT2 _texUV)
{
	SetTexUV(_texUV);
	Init(_fileName);
}

void Square::Init(const char* _fileName, int slices)
{
	CreateMesh(slices);
	CreateMaterial();
	CreateTexture(_fileName);
}

void Square::Init(std::shared_ptr<Texture> tex, int slices)
{
	CreateMesh(slices);
	CreateMaterial();
	LoadTexture(tex);
}

void Square::Update(float dt)
{
	WriteDefShader();
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


void Square::CreateMesh(UINT slices)
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

void Square::WriteDefShader()
{
	if (!mDefPS || !mDefVS)
	{
		DebugLog::LogError("ShaderFile is not set");
		return;
	}

	CameraBase* firstCamera = GameApp::GetCurrentCamera();
	std::shared_ptr<DirLight> dirLight = GameApp::GetComponent<DirLight>("EnvironmentLight");
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
		Vector4{dirLight->GetPosition().x,dirLight->GetPosition().y,dirLight->GetPosition().z,0},
	};

	mDefVS->WriteShader(0, WVP);
	mDefPS->WriteShader(0, &mMaterial.material);
	mDefPS->WriteShader(1, &eyePos);
	mDefPS->WriteShader(2, &light);
}



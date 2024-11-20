#include "Cube.h"
#include "DebugLog.h"
#include "KInput.h"
#include "DirLight.h"
#include "GampApp.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Cube::Cube():Primitive(CUBE)
{
}

void Cube::Init(const char* fileName)
{
	CreateMeshes();
	CreateMaterial();
	CreateTexture(fileName);

}

void Cube::Init(const std::shared_ptr<Texture>& texture)
{
	CreateMeshes();
	CreateMaterial();
	LoadTexture(texture);
}


void Cube::Draw(int texSlot)
{
	SetDefShader();

	for (auto it = mMeshes.begin(); it != mMeshes.end(); ++it)
	{
		mPS->SetShader();
		mVS->SetShader();

		if (texSlot>= 0)
			mPS->SetTexture(texSlot, mMaterial.tex.get());

		it->mesh->Draw();
	}
}

void Cube::Update(float dt)
{
	////Render処理
	//WriteDefShader();
}


void Cube::WriteDefShader()
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
		DirectX::XMFLOAT4 lightAmbient;
		DirectX::XMFLOAT4 lightDiffuse;
		DirectX::XMFLOAT4 lightDir;
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

void Cube::CreateMeshes()
{
	mMeshes.resize(6);
	const float d = 0.5f;
	XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	Vector3 faceNormal[] = {
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(-1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f,-1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f,-1.0f),
	};

	Vector3 pos[] = {
		//+X面
		Vector3(d, -d, -d),
		Vector3(d, d, -d),
		Vector3(d, d, d),
		Vector3(d, -d, d),
		// -X面
		Vector3(-d, -d, d),
		Vector3(-d, d, d),
		Vector3(-d, d, -d),
		Vector3(-d, -d, -d),
		// +Y面
		Vector3(-d, d, -d),
		Vector3(-d, d, d),
		Vector3(d, d, d),
		Vector3(d, d, -d),
		// -Y面
		Vector3(d, -d, -d),
		Vector3(d, -d, d),
		Vector3(-d, -d, d),
		Vector3(-d, -d, -d),
		// +Z面
		Vector3(d, -d, d),
		Vector3(d, d, d),
		Vector3(-d, d, d),
		Vector3(-d, -d, d),
		// -Z面
		Vector3(-d, -d, -d),
		Vector3(-d, d, -d),
		Vector3(d, d, -d),
		Vector3(d, -d, -d),
	};

	std::vector<Vector2> texCoord(4);
	texCoord = {
		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f),
	};

	//VERTEX List
	std::vector<Vertex::VtxPosNormalTex> vtxData;

	vtxData.resize(24);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			vtxData[i * 4 + j] = { pos[i * 4 + j], faceNormal[i], texCoord[j] };
		}
	}

	for (UINT i = 0; i < mMeshes.size(); ++i)
	{
		//std::vector<Vertex::VtxPosColorNormal> vertexData;
		std::vector<Vertex::VtxPosNormalTex> vertexData;
		std::vector<DWORD> indexData;
		// 每个面有4个顶点
		vertexData.resize(4);
		indexData.resize(6);
		for (int j = 0; j < 4; j++)
		{
			vertexData[j] = vtxData[i * 4 + j];
		}
		indexData = {
			0,1,2,2,3,0
		};

		Mesh::MeshData desc = {};
		desc.pVertex = vertexData.data();
		desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
		desc.vertexCount = static_cast<UINT>(vertexData.size());
		desc.pIndex = indexData.data();
		desc.indexSize = sizeof(DWORD);
		desc.indexCount = static_cast<UINT>(indexData.size());
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mMeshes[i].mesh = std::make_unique<Mesh>(desc);
	}
}

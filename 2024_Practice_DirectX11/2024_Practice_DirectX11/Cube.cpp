#include "Cube.h"
#include "DebugLog.h"
#include "KInput.h"
#include "DirLight.h"
#include "GameApp.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Cube::Cube():Primitive(PrimitiveConfig::CUBE)
{
}

void Cube::Init(const char* fileName, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMeshes();
	CreateMaterial();
	CreateTexture(fileName);

}

void Cube::Init(const std::shared_ptr<Texture>& texture, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

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
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}


void Cube::WriteDefShader()
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
		Vector2(0.0f, 1.0f / mUvAnimation->GetSplit().y),
		Vector2(0.0f, 0.0f),
		Vector2(1.f/mUvAnimation->GetSplit().x, 0.0f),
		Vector2(1.0f/mUvAnimation->GetSplit().x, 1.0f/mUvAnimation->GetSplit().y),
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
	SetVertices(vtxData);

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

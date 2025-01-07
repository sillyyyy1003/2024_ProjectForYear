#include "Circle.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

Circle::Circle():Primitive(PrimitiveConfig::CIRCLE)
{
}

void Circle::Update(float dt)
{
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}

void Circle::Draw(int texSlot)
{
	SetDefShader();

	mPS->SetShader();
	mVS->SetShader();
	if (texSlot >= 0)
		mPS->SetTexture(texSlot, mMaterial.tex.get());
	mMesh->Draw();
}


void Circle::Init(const char* _fileName, int slices, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh(slices);
	CreateMaterial();
	CreateTexture(_fileName);
}

void Circle::Init(const char* _fileName, int slices, int levels, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh(levels, slices);
	CreateMaterial();
	CreateTexture(_fileName);

}

void Circle::Init(const std::shared_ptr<Texture>& tex, int slices, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh(slices);
	CreateMaterial();
	LoadTexture(tex);
}

void Circle::Init(const std::shared_ptr<Texture>& tex,int slices, int levels, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh(levels,slices);
	CreateMaterial();
	LoadTexture(tex);
}

void Circle::CreateMesh(UINT slices)
{
	std::vector<Vertex::VtxPosNormalTex> vtx;
	std::vector<DWORD> idx;

	float radius = 0.5f;
	float theta = 0.0f;
	float per_theta = XM_2PI / static_cast<float>(slices);

	const float d = 0.5f;

	//top center
	vtx.push_back({
	Vector3(0.0f, 0.0f, 0.0f),
	Vector3(0.0f, 1.0f, 0.0f),
		Vector2(0.5f,0.5f)
		});


	for (UINT i = 0; i < slices; i++)
	{
		theta = i * per_theta;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		float u = (x / (radius*mUvAnimation->GetSplit().x) + 1.0f) * 0.5f; // Set X [-radius, radius] to [0, 1]
		float v = (z / (radius * mUvAnimation->GetSplit().y) + 1.0f) * 0.5f; // Set Z [-radius, radius] to [0, 1]

		vtx.push_back({
			Vector3(x, 0, z),
			Vector3(0.0f, 1.0f, 0.0f),
			XMFLOAT2(u,v)
			});
	}

	std::vector<DWORD> indexData;

	for (UINT i = 0; i < slices; i++)
	{
		indexData.push_back(0); // CENTER
		indexData.push_back((i + 1) % (slices + 1) + 1); // Next vertex
		indexData.push_back(i + 1); // this vertex
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
	SetVertices(vtx);

}

void Circle::CreateMesh(UINT levels, UINT slices)
{
	std::vector<Vertex::VtxPosNormalTex> vtx;

	float theta = 0.0f;
	float per_theta = XM_2PI / slices;
	float radius = 0.5f;
	float x, z;

	// CENTER
	vtx.push_back({
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector2(0.5f, 0.5f),
		});

	for (UINT i = 0; i < levels; ++i) // 頂点密度
	{
		float r = static_cast<float>(i + 1) / levels * radius;
		for (UINT j = 0; j <= slices; ++j) // 円の縦分割
		{
			theta = per_theta * j;
			x = r * cosf(theta); // x 
			z = r * sinf(theta); // z 

			float u = (x / (radius*mUvAnimation->GetSplit().x) + 1.0f) * 0.5f;
			float v = (z / (radius* mUvAnimation->GetSplit().y) + 1.0f) * 0.5f;

			vtx.push_back({
				Vector3(x, 0.0f, z),
				Vector3(0.0f, 1.0f, 0.0f),
				Vector2(u, v)
			});
		}
	}
	std::vector<DWORD> idx;

	if (levels > 1)
	{
		for (UINT i = 1; i <= slices; ++i)
		{
			idx.push_back(0);
			idx.push_back(i % (slices + 1) + 1);
			idx.push_back(i);
		}
	}

	for (UINT i = 1; i < levels - 1; ++i)
	{
		for (UINT j = 1; j <= slices; ++j)
		{
			idx.push_back((i - 1) * (slices + 1) + j);
			idx.push_back((i - 1) * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(i * (slices + 1) + j % (slices + 1) + 1);

			idx.push_back(i * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i - 1) * (slices + 1) + j);
		}
	}


	//Mesh 
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
	SetIndices(idx);
}



void Circle::WriteDefShader()
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

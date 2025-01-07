#include "Capsule.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

Capsule::Capsule() :Primitive(PrimitiveConfig::CAPSULE)
{
}

void Capsule::Init(const char* filePath, int levels, int slices, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;
	
	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh(levels, slices,1);
	CreateMaterial();
	CreateTexture(filePath);
}

void Capsule::Init(const std::shared_ptr<Texture>& tex, int levels, int slices, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;

	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh(levels, slices, 1);
	CreateMaterial();
	LoadTexture(tex);
}

void Capsule::Update(float dt)
{
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}

void Capsule::Draw(int texSlot)
{
	
	SetDefShader();

	mPS->SetShader();
	mVS->SetShader();

	if (texSlot >= 0)
		mPS->SetTexture(texSlot, mMaterial.tex.get());

	mMesh->Draw();
}

void Capsule::CreateMesh(UINT levels, UINT slices, UINT stacks)
{
	std::vector<Vertex::VtxPosNormalTex> vtx;
	std::vector<DWORD> idx;

	float phi = 0.0f, theta = 0.0f;
	float radius = 0.5f;
	float height = 1.0f;
	float h2 = height / 2.0f;
	float per_phi = XM_PI / levels;
	float per_theta = XM_2PI / slices;
	float stackHeight = height / stacks;

	// Cylinder body vertices
	for (UINT i = 0; i <= stacks; i++)
	{
		float y = -h2 + i * stackHeight;
		for (UINT j = 0; j <= slices; j++)
		{
			theta = j * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);

			float u = theta / (XM_2PI * mUvAnimation->GetSplit().x);
			float v = 1.0f - static_cast<float>(i) / (stacks * mUvAnimation->GetSplit().y);

			vtx.push_back({
				Vector3(x, y, z),
				Vector3(cosf(theta), 0.0f, sinf(theta)),
				Vector2(u,v)
				});
		}
	}

	//Top Semi_Sphere
	vtx.push_back({
		Vector3(0.0f, radius + h2, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector2(0.0f, 0.0f),

		});

	for (UINT i = 1; i < levels / 2; i++)
	{
		phi = per_phi * i;

		// slices+1->スタートとエンドは同じけど、uvは逆
		for (UINT j = 0; j <= slices; j++)
		{
			theta = per_theta * j;
			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi) + h2;
			float z = radius * sinf(phi) * sinf(theta);
			Vector3 pos = { x,y,z };
			Vector3 centerToVertex = { x, y - h2, z }; // Subtract h2 to move the vertex relative to the sphere's center
			Vector3 normal;
			XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&centerToVertex)));

			vtx.push_back({
				pos,
				normal,
				XMFLOAT2(theta / (XM_2PI * mUvAnimation->GetSplit().x), phi / (XM_PI * mUvAnimation->GetSplit().y))
				});
		}
	}

	//Bot Hemisphere
	for (UINT i = levels / 2; i <= levels; i++)
	{
		phi = per_phi * i;

		// slices+1->スタートとエンドは同じけど、uvは逆
		for (UINT j = 0; j <= slices; j++)
		{
			theta = per_theta * j;
			float x = radius * sinf(phi) * cosf(theta);
			float y = -h2 + radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);
			Vector3 pos = { x,y,z };
			Vector3 centerToVertex = { x, y+ h2, z }; // Subtract h2 to move the vertex relative to the sphere's center
			Vector3 normal;
			XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&centerToVertex)));
			
			vtx.push_back({
				pos,
				normal,
				XMFLOAT2(theta / (XM_2PI * mUvAnimation->GetSplit().x), phi / (XM_PI * mUvAnimation->GetSplit().y))
				});
		}
	}

	//Bot point
	vtx.push_back({
		Vector3(0.0f, -h2 - radius, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector2(0.0f, 0.0f),
		});

	//Cylinder
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

	// Offset to move to the top hemisphere vertices
	UINT offset = (slices + 1) * (stacks + 1);

	// Top hemisphere indices
	for (UINT i = 0; i < slices; i++)
	{
		// Top point of the hemisphere
		idx.push_back(offset + 0);
		idx.push_back(offset + i % (slices + 1) + 1);
		idx.push_back(offset + i);
	}

	for (UINT i = 1; i < levels / 2; i++)
	{
		for (UINT j = 1; j <= slices; j++)
		{
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);

			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
		}
	}

	// Offset to move to the bottom hemisphere vertices
	offset += (slices + 1) * (levels / 2);

	// Bottom hemisphere indices
	for (UINT i = 0; i <= levels / 2; i++)
	{
		for (UINT j = 0; j <= slices; j++)
		{
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);

			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
		}
	}

	for (UINT i = 0; i < slices; i++)
	{
		// Bottom point of the hemisphere
		idx.push_back(offset + (levels - 2) * (slices + 1) + i);
		idx.push_back(offset + (levels - 2) * (slices + 1) + i % (slices + 1) + 1);
		idx.push_back(offset + (levels - 1) * (slices + 1) + 1);
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	mMesh = std::make_unique<Mesh>(desc);
	//頂点情報をmVerticesに渡す
	SetVertices(vtx);
}


void Capsule::WriteDefShader()
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



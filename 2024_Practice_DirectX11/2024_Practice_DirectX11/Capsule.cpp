#include "Capsule.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

Capsule::Capsule() :Primitive(CAPSULE)
{
}

void Capsule::Init(const char* filePath)
{
	CreateMesh(64, 64,1);
	CreateMaterial();
	CreateTexture(filePath);
}

void Capsule::Update(float dt)
{
	WriteDefShader();
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

			float u = theta / XM_2PI;
			float v = 1.0f - static_cast<float>(i) / stacks;

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
				XMFLOAT2(theta / XM_2PI, phi / XM_PI)
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
				XMFLOAT2(theta / XM_2PI, phi / XM_PI)
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

void Capsule::CreateMaterial()
{
	mMaterial.material = {
   Color(1.0f, 1.0f, 1.0f, 1.0f),		// 環境光
   Color(1.0f, 1.0f, 1.0f, 1.0f),		// 表面色
   Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
   Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
	};
}

void Capsule::CreateTexture(const char* filePath)
{
	if (!filePath)
	{
		mMaterial.material.isTexEnable = false;
		mMaterial.tex = nullptr;
		return;
	}

	mMaterial.tex = std::make_unique<Texture>();
	HRESULT hr = mMaterial.tex->Create(filePath);
	if (FAILED(hr))
	{
		mMaterial.tex = nullptr;
		mMaterial.material.isTexEnable = false;
	}
	mFilePath = filePath;
}

void Capsule::WriteDefShader()
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



#include "Circle.h"

#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

Circle::Circle():Primitive(CIRCLE)
{
}

void Circle::Update(float dt)
{
	Primitive::Update(dt);
}

void Circle::Draw(int texSlot)
{
	if (isDefShader)
	{
		WriteDefShader();
		mVS = mDefVS.get();
		mPS = mDefPS.get();
	}

	mPS->SetShader();
	mVS->SetShader();
	if (texSlot >= 0)
		mPS->SetTexture(texSlot, mMaterial.tex.get());
	mMesh->Draw();
}

void Circle::SetTexUV(DirectX::XMFLOAT2 _texUV)
{
	Primitive::SetTexUV(_texUV);
}

void Circle::SetScale(const DirectX::XMFLOAT2& scale)
{
	SetScaleXZ(scale);
}

void Circle::Init(const char* _fileName)
{
	CreateMeshes(64);
	CreateMaterial();
	CreateTexture(_fileName);
	LoadDefShader();
}

const void Circle::CreateMeshes(UINT slices)
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

	//top cap
	for (UINT i = 0; i <= slices; i++)
	{
		theta = i * per_theta;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		float u = (x / radius + 1.0f) * 0.5f; // 将 x 范围 [-radius, radius] 映射到 [0, 1]
		float v = (z / radius + 1.0f) * 0.5f; // 将 z 范围 [-radius, radius] 映射到 [0, 1]

		vtx.push_back({
			Vector3(x, 0, z),
			Vector3(0.0f, 1.0f, 0.0f),
			XMFLOAT2(u,v)
			});
	}

	std::vector<DWORD> indexData;

	for (UINT i = 0; i <= slices; i++)
	{
		indexData.push_back(0); // 圆心
		indexData.push_back((i + 1) % (slices + 1) + 1); // 下一个边缘点
		indexData.push_back(i + 1); // 当前边缘点
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

const void Circle::CreateMaterial()
{
	mMaterial.material =
	{
		Color(1.0f, 1.0f, 1.0, 1.0f),		// 環境光
		Color(1.0f, 1.0f, 1.0, 1.0f),		// 表面色
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
	};

}

const void Circle::CreateTexture(const char* _fileName)
{
	if (!_fileName)
	{
		mMaterial.material.isTexEnable = false;
		mMaterial.tex = nullptr;
		return;
	}

	mMaterial.tex = std::make_unique<Texture>();
	HRESULT hr = mMaterial.tex->Create(_fileName);
	if (FAILED(hr))
	{
		mMaterial.tex = nullptr;
		mMaterial.material.isTexEnable = false;
	}
	mFilePath = _fileName;
}

void Circle::WriteDefShader()
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

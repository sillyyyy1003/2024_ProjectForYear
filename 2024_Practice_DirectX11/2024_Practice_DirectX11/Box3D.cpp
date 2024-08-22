#include "Box3D.h"
#include "D3DApp.h"
#include "DebugLog.h"
#include "KInput.h"
#include "CameraDCC.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Box3D::InitResource(const char* fileName)
{
	CreateMeshes();
	CreateTexture(fileName);
	CreateMaterial();
}

void Box3D::SetMaterial(Material _material, int materialID)
{
	mMaterials[materialID] = _material;
}

void Box3D::Draw(int texSlot)
{
	for (auto it = mMeshes.begin(); it != mMeshes.end(); ++it)
	{
		mPS->SetShader();
		mVS->SetShader();

		if (texSlot>= 0)
			mPS->SetTexture(texSlot, mTex.get());

		it->mesh->Draw();
	}
}

void Box3D::Update()
{


}

void Box3D::SetPixelShader(std::shared_ptr<PixelShader> ps)
{
	mPS = ps;
}

void Box3D::SetVertexShader(std::shared_ptr<VertexShader> vs)
{
	mVS = vs;
}

void Box3D::CreateMaterial(int matNum)
{
	mMaterials.resize(matNum);

	for (int i = 0; i < mMaterials.size(); i++)
	{
		mMaterials[i] = {
			Color(1.0f, 1.0f, 1.0, 1.0f),		// 環境光
			Color(1.0f, 1.0f, 1.0, 1.0f),		// 表面色
			Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
			Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
		};
	}
}

void Box3D::CreateTexture(const char* fileName)
{
	mTex = std::make_shared<Texture>();
	mTex->Create(fileName);
}

void Box3D::CreateMeshes()
{
	mMeshes.resize(6);
	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 faceNormal[] = {
		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f,-1.0f),
	};

	DirectX::XMFLOAT3 pos[] = {
		//+X面
		XMFLOAT3(d, -d, -d),
		XMFLOAT3(d, d, -d),
		XMFLOAT3(d, d, d),
		XMFLOAT3(d, -d, d),
		// -X面
		XMFLOAT3(-d, -d, d),
		XMFLOAT3(-d, d, d),
		XMFLOAT3(-d, d, -d),
		XMFLOAT3(-d, -d, -d),
		// +Y面
		XMFLOAT3(-d, d, -d),
		XMFLOAT3(-d, d, d),
		XMFLOAT3(d, d, d),
		XMFLOAT3(d, d, -d),
		// -Y面
		XMFLOAT3(d, -d, -d),
		XMFLOAT3(d, -d, d),
		XMFLOAT3(-d, -d, d),
		XMFLOAT3(-d, -d, -d),
		// +Z面
		XMFLOAT3(d, -d, d),
		XMFLOAT3(d, d, d),
		XMFLOAT3(-d, d, d),
		XMFLOAT3(-d, -d, d),
		// -Z面
		XMFLOAT3(-d, -d, -d),
		XMFLOAT3(-d, d, -d),
		XMFLOAT3(d, d, -d),
		XMFLOAT3(d, -d, -d),
	};

	std::vector<DirectX::XMFLOAT2> texCoord(4);
	texCoord = {
		DirectX::XMFLOAT2(0.0f, 1.0f),
		DirectX::XMFLOAT2(0.0f, 0.0f),
		DirectX::XMFLOAT2(1.0f, 0.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f),
	};

	mVtxData.resize(24);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mVtxData[i * 4 + j] = { pos[i * 4 + j], faceNormal[i], texCoord[j] };
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
			vertexData[j] = mVtxData[i * 4 + j];
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
		mMeshes[i].mesh = std::make_shared<Mesh>(desc);
		mMeshes[i].materialID = i;
	}
}

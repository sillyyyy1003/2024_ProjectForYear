#include "Plane3D.h"

#include "CameraDCC.h"
#include "D3DUtil.h"

using namespace DirectX;
using namespace SimpleMath;


void Plane3D::InitResource(const char* _fileName)
{
	CreateMeshes();
	CreateMaterial();
	CreateTexture(_fileName);

	CreatePS();
	CreateVS();

}

void Plane3D::Update(float dt)
{

}

void Plane3D::Draw(int texSlot)
{
	for (int i = 0; i < mMeshes.size(); ++i)
	{
		mPS->SetShader();
		mVS->SetShader();
		if (texSlot >= 0)
			mPS->SetTexture(texSlot, mTex.get());

		mMeshes[i].mesh->Draw();
	}
}

const void Plane3D::SetPixelShader(std::shared_ptr<PixelShader> ps)
{
	mPS = ps;
}

const void Plane3D::SetVertexShader(std::shared_ptr<VertexShader> vs)
{
	mVS = vs;
}

const void Plane3D::SetSize(float width, float height)
{
	mTransform.SetScale(width, 1.0f, height);
}


const void Plane3D::CreatePS()
{
	mPS = std::make_shared<PixelShader>(ShaderEnum::Pixel);
}

const void Plane3D::CreateVS()
{
	mVS = std::make_shared<VertexShader>(ShaderEnum::Vertex);

}

const void Plane3D::CreateMeshes()
{
	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 pos[] = {

		XMFLOAT3(-d, 0, -d),
		XMFLOAT3(-d, 0, d),
		XMFLOAT3(d, 0, d),
		XMFLOAT3(d, 0, -d),

		XMFLOAT3(d, 0, -d),
		XMFLOAT3(d, 0, d),
		XMFLOAT3(-d, 0, d),
		XMFLOAT3(-d,0, -d),

	};

	//todo:if use tangent need to fix the vertex
	std::vector<Vertex::VtxPosNormalTex> vtx;
	vtx.resize(8);

	vtx = {
		//+y
		{pos[0],Vector3(0.0f,1.0f,0.0f),Vector2(0.f,0.f)},
		{pos[1],Vector3(0.0f,1.0f,0.0f),Vector2(1.f,0.f)},
		{pos[2],Vector3(0.0f,1.0f,0.0f),Vector2(1.f,1.f)},
		{pos[3],Vector3(0.0f,1.0f,0.0f),Vector2(0.f,1.f)},
		//-Y
		{pos[4],Vector3(0.0f,-1.0f,0.0f),Vector2(0.f,1.f)},
		{pos[5],Vector3(0.0f,-1.0f,0.0f),Vector2(1.f,1.f)},
		{pos[6],Vector3(0.0f,-1.0f,0.0f),Vector2(1.f,0.f)},
		{pos[7],Vector3(0.0f,-1.0f,0.0f),Vector2(0.f,0.f)},

	};
	
	mMeshes.resize(2);
	for (int i = 0; i < 2; ++i)
	{
		std::vector<Vertex::VtxPosNormalTex> vertexData;
		std::vector<DWORD> indexData;
		// 每个面有4个顶点
		vertexData.resize(4);
		indexData.resize(6);

		for (int j = 0; j < 4; j++)
		{
			vertexData[j] = vtx[i * 4 + j];
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
	}
	
}

const void Plane3D::CreateMaterial(int matNum)
{
	mMaterials.resize(matNum);

	for (int i = 0; i < mMaterials.size(); i++)
	{
		mMaterials[i] = {
			Color(0.2f, 0.2f, 0.2f, 1.0f),		// 弱い環境光
			Color(0.7f, 0.3f, 0.5f, 1.0f),		// 表面色
			Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
			Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
		};
	}

}

const void Plane3D::CreateTexture(const char* _fileName)
{
	mTex = std::make_shared<Texture>();
	mTex->Create(_fileName);
	
}


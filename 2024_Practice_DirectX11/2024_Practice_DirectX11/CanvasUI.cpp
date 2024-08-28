#include "CanvasUI.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Texture.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void CanvasUI::InitCanvas(const char* _fileName)
{
	CreateMeshBuffer();
	CreateMaterial(_fileName);

	mPS = std::make_unique<PixelShader>();
	mVS = std::make_unique<VertexShader>();

	HR(mPS->LoadShaderFile("Assets/Shader/PS_UI.cso"));
	HR(mVS->LoadShaderFile("Assets/Shader/VS_UI.cso"));

}

void CanvasUI::Update(float dt)
{

}

void CanvasUI::Draw()
{

	//============================================
	//Generate Matrix
	//============================================
	XMMATRIX WVP[3]={};
	WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
	WVP[2] = XMMatrixOrthographicLH(16.0f, 9.0f, 0.0f, 3.0f);
	WVP[2] = XMMatrixTranspose(WVP[2]);

	mVS->WriteShader(0, WVP);
	mPS->WriteShader(0, &(mMaterial.material));

	mVS->SetShader();
	mPS->SetShader();
	if(mMaterial.tex)
		mPS->SetTexture(0, mMaterial.tex.get());
	mMesh.mesh->Draw();
}

void CanvasUI::CreateMeshBuffer()
{

	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 pos[] = {

		XMFLOAT3(-d, d, 0),	//left top
		XMFLOAT3(d, d, 0),	//right top
		XMFLOAT3(d, -d,0),	//right bot
		XMFLOAT3(-d, -d, 0)	//left bot
	};

	std::vector<Vertex::VtxPosNormalTex> vtx;
	vtx.resize(4);

	vtx = {
		//+y
		{pos[0],Vector3(0.0f,0.0f,-1.0f),Vector2(0.f,0.f)},
		{pos[1],Vector3(0.0f,0.0F,-1.0f),Vector2(1.f,0.f)},
		{pos[2],Vector3(0.0f,0.0F,-1.0f),Vector2(1.f,1.f)},
		{pos[3],Vector3(0.0f,0.0F,-1.0f),Vector2(0.f,1.f)},
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
	mMesh.mesh = std::make_unique<Mesh>(desc);

}

void CanvasUI::CreateTexture(const char* _fileName)
{
	mMaterial.tex = std::make_unique<Texture>();
	mMaterial.tex->Create(_fileName);
}

void CanvasUI::CreateMaterial(const char* _fileName)
{
	mMaterial.material = {
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// 環境光
		Color(0.2f, 0.2f, 0.2f, 1.0f),		// 表面色
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
	};
	CreateTexture(_fileName);

}



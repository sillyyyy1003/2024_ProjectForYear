#include "CanvasUI.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "Shader.h"
#include "Texture.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

CanvasUI::~CanvasUI()
{
}

void CanvasUI::InitResource(const char* _fileName, DirectX::XMFLOAT2 size, bool isDefaultSize)
{
	CreateMeshBuffer();
	CreateMaterial(_fileName);
	LoadShader();

	

	if (isDefaultSize)
	{
		SetSize(static_cast<float>(mMaterial.tex->GetWidth()), static_cast<float>(mMaterial.tex->GetHeight()));
	}
	else
	{
		SetSize(size.x, size.y);
	}


}

void CanvasUI::InitData(DirectX::XMFLOAT3 pos)
{
	mTransform.SetPosition(pos);
	mOriginPos = { pos.x,pos.y };
}

void CanvasUI::Update(float dt)
{
	UpdateScale();
	UpdatePos();
}

void CanvasUI::Draw()
{

	//============================================
	//Generate Matrix
	//============================================

	WriteShader();

	//Bind Shader
	mDefVS->SetShader();
	mDefPS->SetShader();
	if(mMaterial.tex)
		mDefPS->SetTexture(0, mMaterial.tex.get());
	mMesh->Draw();
}

void CanvasUI::SetSize(float x, float y)
{
	//todo：プロトタイプ段階、テクスチャは全部1920*1080サイズ　
	//オブジェクトのサイズの不具合はとりあえず強制修正で行くあとで直す
	x *= 0.6666f;
	y *= 0.6666f;

	mTransform.SetScale(x, y, 1.0f);
	mOriginScale = { x,y };
}

void CanvasUI::SetPosZ(float z)
{
	mTransform.SetPositionZ(z);
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
	mMesh = std::make_unique<Mesh>(desc);

}

void CanvasUI::CreateTexture(const char* _fileName)
{
	if(!_fileName)
	{
		mMaterial.material.isTexEnable = false;
		mMaterial.tex = nullptr;
		return;
	}

	mMaterial.tex = std::make_unique<Texture>();
	mMaterial.tex->Create(_fileName);
}

void CanvasUI::CreateMaterial(const char* _fileName)
{
	mMaterial.material = {
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// 環境光
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// 表面色
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
	};
	CreateTexture(_fileName);
	mTexResource = _fileName;
}

void CanvasUI::LoadShader()
{
	mDefPS = std::make_unique<PixelShader>();
	mDefVS = std::make_unique<VertexShader>();

	HR(mDefPS->LoadShaderFile("Assets/Shader/PS_DefaultUI.cso"));
	HR(mDefVS->LoadShaderFile("Assets/Shader/VS_DefaultUI.cso"));

}

void CanvasUI::UpdateScale()
{
	
	float viewWidth = static_cast<float>(gD3D->GetWinWidth());
	float viewHeight = static_cast<float>(gD3D->GetWinHeight());

	if(gD3D->GetResized())
	{
		Vector3 ratio = { viewWidth / WIN_WIDTH,viewHeight / WIN_HEIGHT,1.0f };
		Vector3 scale = { ratio.x * mOriginScale.x,ratio.y * mOriginScale.y,1.0f };
		mTransform.SetScale(scale);
	}
}

void CanvasUI::UpdatePos()
{
	float viewWidth = static_cast<float>(gD3D->GetWinWidth());
	float viewHeight = static_cast<float>(gD3D->GetWinHeight());

	if (gD3D->GetResized())
	{
		Vector3 ratio = { viewWidth / WIN_WIDTH,viewHeight / WIN_HEIGHT,1.0f };
		Vector3 pos = {mOriginPos.x,mOriginPos.y, mTransform.GetPosition().z};
		pos *= ratio;
		mTransform.SetPosition(pos);
	}

}

void CanvasUI::WriteShader()
{
	Vector2 viewSize = { static_cast<float>(gD3D->GetWinWidth()),static_cast<float>(gD3D->GetWinHeight()) };

	XMMATRIX WVP[3] = {};
	WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
	WVP[2] = XMMatrixOrthographicLH(viewSize.x, viewSize.y, 0.0f, 3.0f);
	WVP[2] = XMMatrixTranspose(WVP[2]);

	//Write Shader
	mDefVS->WriteShader(0, WVP);
	mDefPS->WriteShader(0, &(mMaterial.material));
}



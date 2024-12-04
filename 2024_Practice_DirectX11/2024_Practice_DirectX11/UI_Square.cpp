#include "UI_Square.h"

#include "GameApp.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

UI_Square::UI_Square():UI_Primitive(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE)
{

}

void UI_Square::Init(const char* _fileName, int slices, DirectX::XMINT2 _UVSplit)
{
	//UV Animation を使うか
	InitAnimation(_UVSplit);
	if (slices == 0)
	{
		CreateMesh();
	}
	else
	{
		CreateMesh(slices);
	}
	CreateMaterial();
	CreateTexture(_fileName);
	
}

void UI_Square::Init(const std::shared_ptr<Texture>& tex, int slices, DirectX::XMINT2 _UVSplit)
{
	InitAnimation(_UVSplit);

	if (slices == 0)
	{
		CreateMesh();
	}
	else
	{
		CreateMesh(slices);
	}
	CreateMaterial();
	CreateTexture(tex);
}




void UI_Square::CreateMesh(int slices)
{
	float size = 1.0f;
	float halfSize = size / 2;
	float step = size / slices;
	std::vector<Vertex::VtxPosNormalTex> vtx;

	for (int i = 0; i <= slices; i++)
	{
		for (int j = 0; j <= slices; j++)
		{
			float x = -halfSize + j * step; // x 
			float y = -halfSize + i * step; // z 
			float u = static_cast<float>(j) / (slices * mUvAnimation->GetSplit().x); // u
			float v = static_cast<float>(i) / (slices * mUvAnimation->GetSplit().y); // v

			vtx.push_back({ XMFLOAT3(x, y, 0), Vector3(0.0f, 0.0, -1.0f), Vector2(u, v) });
		}
	}

	std::vector<DWORD> indexData;
	for (int i = 0; i < slices; i++)
	{
		for (int j = 0; j < slices; j++)
		{
			int start = i * (slices + 1) + j;
			indexData.push_back(start);               // 左上
			indexData.push_back(start + slices + 1);  // 左下
			indexData.push_back(start + 1);           // 右上

			indexData.push_back(start + 1);           // 右上
			indexData.push_back(start + slices + 1);  // 左下
			indexData.push_back(start + slices + 2);  // 右下
		}
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

void UI_Square::CreateMesh()
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
		{pos[1],Vector3(0.0f,0.0f,-1.0f),Vector2(1.f / mUvAnimation->GetSplit().x,0.f)},
		{pos[2],Vector3(0.0f,0.0f,-1.0f),Vector2(1.f / mUvAnimation->GetSplit().x,1.f / mUvAnimation->GetSplit().y)},
		{pos[3],Vector3(0.0f,0.0f,-1.0f),Vector2(0.f,1.f / mUvAnimation->GetSplit().y)},
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
	SetVertices(vtx);
}

void UI_Square::Update()
{
	UI_Primitive::Update();
}

void UI_Square::Draw(int texSlot)
{
	SetDefShader();
	mVS->SetShader();
	mPS->SetShader();
	if (mMaterial.tex)
		mDefPS->SetTexture(0, mMaterial.tex.get());
	mMesh->Draw();

}

void UI_Square::WriteDefShader()
{
	Vector2 viewSize = { static_cast<float>(gD3D->GetWinWidth()),static_cast<float>(gD3D->GetWinHeight()) };

	XMMATRIX WVP[3] = {};
	WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
	WVP[2] = XMMatrixOrthographicLH(viewSize.x, viewSize.y, 0.0f, 3.0f);
	WVP[2] = XMMatrixTranspose(WVP[2]);

	UVConstantBuffer uvBuffer;
	uvBuffer.useUV = isUseUVAnimation;
	//UV MATRIX 作成
	if (isUseUVAnimation)
	{
		uvBuffer.uv = XMMatrixTranslation(mUvAnimation->GetOffsetUV().x, mUvAnimation->GetOffsetUV().y, 0.0f);
		uvBuffer.uv = XMMatrixTranspose(uvBuffer.uv);
	}

	//Write Shader
	mDefVS->WriteShader(0, WVP);
	mDefVS->WriteShader(1, &uvBuffer);
	mDefPS->WriteShader(0, &(mMaterial.material));
}

void UI_Square::InitAnimation(DirectX::XMINT2 _split)
{
	if (_split.x == 1 && _split.y == 1)
	{
		isUseUVAnimation = false;
	}
	else
	{
		isUseUVAnimation = true;
	}
	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_split);
}


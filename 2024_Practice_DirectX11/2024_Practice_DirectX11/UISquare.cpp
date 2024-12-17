#include "UISquare.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

UISquare::UISquare() :UI_Primitive(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE)
{
}

void UISquare::Init(const std::shared_ptr<Texture>& tex, DirectX::XMINT2 _UVSplit)
{
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
	{
		isUseUVAnimation = false;
	}
	else
	{
		isUseUVAnimation = true;
	}
	//UV AnimationÇÃèâä˙âª
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);

	CreateMesh();
	CreateMaterial();
	CreateTexture(tex);
	LoadDefShader();
}

void UISquare::Draw(int texSlot)
{
	SetDefShader();
	mVS->SetShader();
	mPS->SetShader();
	if (mMaterial.tex)
		mDefPS->SetTexture(0, mMaterial.tex.get());
	mMesh->Draw();
}

void UISquare::CreateMesh()
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

void UISquare::Update()
{
	UI_Primitive::Update();
}

void UISquare::WriteDefShader()
{
	XMMATRIX WVP[3] = {};
	WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
	WVP[2] = XMMatrixOrthographicLH(WIN_WIDTH, WIN_HEIGHT, 0.0f, 3.0f);
	WVP[2] = XMMatrixTranspose(WVP[2]);

	UVConstantBuffer uvBuffer;
	uvBuffer.useUV = isUseUVAnimation;
	//UV MATRIX çÏê¨
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

json UISquare::SaveData()
{
	json data;
	data["Position"] = {mTransform.GetPosition().x,mTransform.GetPosition().y,mTransform.GetPosition().z};
	data["Scale"] = { mTransform.GetScale().x,mTransform.GetScale().y,mTransform.GetScale().z };
	data["Rotation"] = { mTransform.GetRotationInRadian().x,mTransform.GetRotationInRadian().y,mTransform.GetRotationInRadian().z };
	data["Diffuse"] = { GetDiffuseColor().x,GetDiffuseColor().y,GetDiffuseColor().z,GetDiffuseColor().w };
	return data;
}

void UISquare::LoadSaveData(json data)
{
	float pos[3] = { data["Position"][0], data["Position"][1],data["Position"][2] };
	SetPosition(pos[0],pos[1]);
	SetPosZ(pos[2]);

	float scale[3] = { data["Scale"][0],data["Scale"][1] ,data["Scale"][2] };
	SetScale(scale[0], scale[1]);

	float rotation[3] = { data["Rotation"][0],data["Rotation"][1] ,data["Rotation"][2] };
	mTransform.SetRotationInRadian(rotation);

	float color[4] = { data["Diffuse"][0],data["Diffuse"][1] ,data["Diffuse"][2] ,data["Diffuse"][3] };
	SetDiffuseColor(color);

}



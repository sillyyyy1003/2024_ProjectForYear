#include "Sprite.h"
#include <memory>
using namespace DirectX;
using namespace DirectX::SimpleMath;

Sprite::Data Sprite::mData;
std::shared_ptr<VertexShader> Sprite::mDefVS;
std::shared_ptr<PixelShader> Sprite::mDefPS;
std::shared_ptr<Texture> Sprite::mWhiteTex;
bool Sprite::isDefShader;

void Sprite::Init()
{
	std::vector<Vertex::VtxPosTex> vertices;
	vertices.resize(4);

	vertices = {
		{Vector3{-0.5f, 0.5f, 0.0f}, Vector2{0.0f, 0.0f}},
		{Vector3{ 0.5f, 0.5f, 0.0f}, Vector2{1.0f, 0.0f}},
		{Vector3{-0.5f,-0.5f, 0.0f}, Vector2{0.0f, 1.0f}},
		{Vector3{ 0.5f,-0.5f, 0.0f}, Vector2{1.0f, 1.0f}},
	};

	Mesh::MeshData desc = {};
	desc.pVertex = vertices.data();
	desc.vertexSize = sizeof(Vertex::VtxPosTex);
	desc.vertexCount =(UINT)vertices.size();
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	mData.mesh = std::make_shared<Mesh>(desc);


	//Shader
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();

	mDefPS->LoadShaderFile("Assets/Shader/PS_DefaultSprite.cso");
	mDefVS->LoadShaderFile("Assets/Shader/VS_DefaultSprite.cso");


	//Param&Matrix
	mData.Param[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	mData.Param[1] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	mData.Param[2] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&mData.WVP[0], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mData.WVP[1], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mData.WVP[2], DirectX::XMMatrixIdentity());


	//Default Texture
	BYTE color[] = { 255,255,255,255 };
	mWhiteTex = std::make_shared<Texture>();
	mWhiteTex->Create(DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, color);

}

void Sprite::UnInit()
{
}

void Sprite::Draw()
{
	if(isDefShader)
	{
		mData.VS = mDefVS.get();
		mData.PS = mDefPS.get();
	}

	mData.VS->WriteShader(0, mData.WVP);
	mData.VS->WriteShader(1, mData.Param);
	mData.VS->SetShader();
	mData.PS->SetTexture(0, mData.tex.get());
	mData.PS->SetShader();
	mData.mesh->Draw();


}

void Sprite::SetVertexShader(VertexShader* shader)
{
	isDefShader = false;
	mData.VS = shader;
}

void Sprite::SetPixelShader(PixelShader* shader)
{
	isDefShader = false;
	mData.PS = shader;

}

void Sprite::SetOffset(DirectX::XMFLOAT2 offset)
{
	mData.Param[0].x = offset.x;
	mData.Param[0].y = offset.y;
}

void Sprite::SetSize(DirectX::XMFLOAT2 size)
{
	mData.Param[0].z = size.x;
	mData.Param[0].w = size.y;
}

void Sprite::SetUVPos(DirectX::XMFLOAT2 pos)
{
	mData.Param[1].x = pos.x;
	mData.Param[1].y = pos.y;
}

void Sprite::SetUVScale(DirectX::XMFLOAT2 scale)
{
	mData.Param[1].x = scale.x;
	mData.Param[1].y = scale.y;
}

void Sprite::SetColor(DirectX::XMFLOAT4 color)
{
	mData.Param[2] = color;
}

void Sprite::SetTexture(const std::shared_ptr<Texture>& tex)
{
	mData.tex = tex;
}

void Sprite::SetWorld(DirectX::XMFLOAT4X4 world)
{
	mData.WVP[0] = world;
}

void Sprite::SetView(DirectX::XMFLOAT4X4 view)
{
	mData.WVP[1] = view;
}

void Sprite::SetProjection(DirectX::XMFLOAT4X4 proj)
{
	mData.WVP[2] = proj;
}

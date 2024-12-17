#include "UI_Capsule.h"

#include "DirLight.h"
#include "GameApp.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

UI_Capsule::UI_Capsule():UI_Primitive(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE)
{
}

void UI_Capsule::Init(const std::shared_ptr<Texture>& tex, DirectX::XMFLOAT2 _squareSize, int slices, DirectX::XMINT2 _split)
{
    //UV Animation を使うか
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
    mSquareSize = _squareSize;
    //Init mesh/material/Texture
    CreateMesh(slices);
    CreateMaterial();
    CreateTexture(tex);
}

void UI_Capsule::Init(const char* filePath, DirectX::XMFLOAT2 _squareSize, int slices, DirectX::XMINT2 _split)
{
    //UV Animation を使うか
    if (_split.x == 1 && _split.y == 1)
        isUseUVAnimation = false;
    else
        isUseUVAnimation = true;
    
    //UV Animationの初期化
    mUvAnimation = std::make_unique<UVAnimation>();
    mUvAnimation->Init(_split);
    mSquareSize = _squareSize;
	CreateMesh(slices);
    CreateMaterial();
    CreateTexture(filePath);
  
}

void UI_Capsule::Update()
{
    //Update size & pos
    UI_Primitive::Update();

}

void UI_Capsule::Draw(int texSlot)
{
    SetDefShader();
    mVS->SetShader();
    mPS->SetShader();
    if (mMaterial.tex)
        mDefPS->SetTexture(0, mMaterial.tex.get());
    mMesh->Draw();
}

void UI_Capsule::SetScale(const DirectX::XMFLOAT2& size)
{
   	mSquareSize = size;
	mVertices.clear();

    mSquareSize = size;
    mVertices = CreateNewVertexData();
    mMesh->Write(mVertices.data());
}

void UI_Capsule::SetScale(float scale)
{
	UI_Primitive::SetScale(scale);
    mOriginScale={scale, scale};
}


void UI_Capsule::CreateMesh(int slices)
{
  
    float radius = mSquareSize.y / 2.0f; //半圆的半径
    float theta = 0.0f;
    float per_theta = DirectX::XM_PI / static_cast<float>(slices);
    float length = mSquareSize.x; //中间矩形的宽度
    std::vector<Vertex::VtxPosNormalTex> vtx;
    std::vector<DWORD> indexData;


    vtx.push_back({ Vector3(-length / 2, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.5f) }); 
    for (int i = 0; i <= slices; i++)
    {
        theta = i * per_theta - DirectX::XM_PIDIV2;
        float x = -length / 2 - radius * cosf(theta);
        float y = radius * sinf(theta);
        float u = (x + length / 2) / length;
        float v = (y / radius + 1.0f) * 0.5f;

        vtx.push_back({ Vector3(x, y, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(u, 1 - v) });
    }


    for (int i = 0; i <= slices; i++)
    {
        float x = -length / 2 + i * (length / slices);
        vtx.push_back({ Vector3(x, radius, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(x / length + 0.5f, 0.0f) });
        vtx.push_back({ Vector3(x, -radius, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(x / length + 0.5f, 1.0f) });
    }

  
    int rightCenterIndex = static_cast<int>(vtx.size());
    vtx.push_back({ Vector3(length / 2, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.5f) }); 
    for (int i = 0; i <= slices; i++)
    {
        theta = i * per_theta - DirectX::XM_PIDIV2;
        float x = length / 2 + radius * cosf(theta);
        float y = radius * sinf(theta);
        float u = (x + length / 2) / length;
        float v = (y / radius + 1.0f) * 0.5f;

        vtx.push_back({ Vector3(x, y, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(u, 1 - v) });
    }

    // 左半圆索引
    for (int i = 0; i < slices; i++)
    {
        indexData.push_back(0);                  
        indexData.push_back(i + 1);            
        indexData.push_back(i + 2);
    }

    int rectStart = slices + 1;
    for (int i = 0; i <= slices; i++)
    {
        indexData.push_back(rectStart + 2 * i);      // 左上
        indexData.push_back(rectStart + 2 * i + 1);  // 左下
        indexData.push_back(rectStart + 2 * i + 2);  // 右上

        indexData.push_back(rectStart + 2 * i + 1);  // 左下
        indexData.push_back(rectStart + 2 * i + 3);  // 右下
        indexData.push_back(rectStart + 2 * i + 2);  // 右上
    }

    // 右半圆
    int rightStart = rightCenterIndex + 1;
    for (int i = 0; i <= slices; i++)
    {
        indexData.push_back(rightCenterIndex);         // 右半圆中心
        indexData.push_back(rightStart + i + 1);       // 当前半圆顶点
        indexData.push_back(rightStart + i);           // 上一个半圆顶点
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


void UI_Capsule::WriteDefShader()
{
    XMMATRIX WVP[3] = {};
    WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
    WVP[2] = XMMatrixOrthographicLH(WIN_WIDTH, WIN_HEIGHT, 0.0f, 3.0f);
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

std::vector<Vertex::VtxPosNormalTex> UI_Capsule::CreateNewVertexData(int slices)
{
    float radius = mSquareSize.y / 2.0f;
    float theta = 0.0f;
    float per_theta = DirectX::XM_PI / static_cast<float>(slices);
    float length = mSquareSize.x;
    std::vector<Vertex::VtxPosNormalTex> vtx;
    std::vector<DWORD> indexData;


    vtx.push_back({ Vector3(-length / 2, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.5f) });
    for (int i = 0; i <= slices; i++)
    {
        theta = i * per_theta - DirectX::XM_PIDIV2;
        float x = -length / 2 - radius * cosf(theta);
        float y = radius * sinf(theta);
        float u = (x + length / 2) / length;
        float v = (y / radius + 1.0f) * 0.5f;

        vtx.push_back({ Vector3(x, y, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(u, 1 - v) });
    }


    for (int i = 0; i <= slices; i++)
    {
        float x = -length / 2 + i * (length / slices);
        vtx.push_back({ Vector3(x, radius, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(x / length + 0.5f, 0.0f) });
        vtx.push_back({ Vector3(x, -radius, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(x / length + 0.5f, 1.0f) });
    }

    vtx.push_back({ Vector3(length / 2, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.5f) });
    for (int i = 0; i <= slices; i++)
    {
        theta = i * per_theta - DirectX::XM_PIDIV2;
        float x = length / 2 + radius * cosf(theta);
        float y = radius * sinf(theta);
        float u = (x + length / 2) / length;
        float v = (y / radius + 1.0f) * 0.5f;

        vtx.push_back({ Vector3(x, y, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(u, 1 - v) });
    }
    return vtx;
}

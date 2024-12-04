#include "Sphere.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

Sphere::Sphere() :Primitive(PrimitiveConfig::SPHERE)
{
}

void Sphere::Init(const char* _fileName, int levels, int slices,DirectX::XMINT2 _UVSplit)
{
    //UV Animation を使うか
    if (_UVSplit.x == 1 && _UVSplit.y == 1)
        isUseUVAnimation = false;
    else
        isUseUVAnimation = true;

    //UV Animationの初期化
    mUvAnimation = std::make_unique<UVAnimation>();
    mUvAnimation->Init(_UVSplit);

    CreateMesh(levels, slices);
	CreateMaterial();
    CreateTexture(_fileName);
}

void Sphere::Init(const std::shared_ptr<Texture>& tex, int levels, int slices, DirectX::XMINT2 _UVSplit)
{
    //UV Animation を使うか
    if (_UVSplit.x == 1 && _UVSplit.y == 1)
        isUseUVAnimation = false;
    else
        isUseUVAnimation = true;

    //UV Animationの初期化
    mUvAnimation = std::make_unique<UVAnimation>();
    mUvAnimation->Init(_UVSplit);

    CreateMesh(levels, slices);
    CreateMaterial();
    LoadTexture(tex);
}

void Sphere::Update(float dt)
{
    if (!isUseUVAnimation) { return; }
    mUvAnimation->UpdateUV();
}


void Sphere::Draw(int texSlot)
{
    SetDefShader();
   
    mPS->SetShader();
    mVS->SetShader();

    if (texSlot >= 0)
	   mPS->SetTexture(texSlot, mMaterial.tex.get());

	mMesh->Draw();
}

void Sphere::CreateMesh(UINT levels, UINT slices)
{
    std::vector<Vertex::VtxPosNormalTex> vtx;

    float phi = 0.0f, theta = 0.0f;
    float per_phi = XM_PI / levels;
    float per_theta = XM_2PI / slices;
    float x, y, z;
    float radius = 0.5f;

    // Generate vertices

    //上の頂点
    vtx.push_back({
        Vector3(0.0f, radius, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector2(0.0f, 0.0f),

    });
    //他
    for (UINT i = 1; i < levels; ++i)
    {
        phi = per_phi * i;

        // slices+1->スタートとエンドは同じけど、uvは逆
        for (UINT j = 0; j <= slices; ++j)
        {
            theta = per_theta * j;
            x = radius * sinf(phi) * cosf(theta);
            y = radius * cosf(phi);
            z = radius * sinf(phi) * sinf(theta);
            Vector3 pos = { x,y,z };
            Vector3 normal;
            XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

            vtx.push_back({
                pos,
                normal,
                XMFLOAT2(theta / (XM_2PI*mUvAnimation->GetSplit().x), phi / (XM_PI*mUvAnimation->GetSplit().y))
            });
        }
    }
    //下の頂点
    vtx.push_back({
     Vector3(0.0f, -radius, 0.0f),
     Vector3(0.0f, -1.0f, 0.0f),
     Vector2(0.0f, 1.0f),
        });



    std::vector<DWORD> idx;

    if (levels > 1)
    {
        for (UINT i = 1; i <= slices; ++i)
        {
            idx.push_back(0);
            idx.push_back(i % (slices + 1) + 1);
            idx.push_back(i);
        }
    }

    for (UINT i = 1; i < levels - 1; ++i)
    {
        for (UINT j = 1; j <= slices; ++j)
        {
            idx.push_back((i - 1) * (slices + 1) + j);
            idx.push_back((i - 1) * (slices + 1) + j % (slices + 1) + 1);
            idx.push_back(i * (slices + 1) + j % (slices + 1) + 1);

            idx.push_back(i * (slices + 1) + j % (slices + 1) + 1);
            idx.push_back(i * (slices + 1) + j);
            idx.push_back((i - 1) * (slices + 1) + j);
        }
    }

    if (levels > 1)
    {
        for (UINT i = 1; i <= slices; ++i)
        {
            idx.push_back((levels - 2) * (slices + 1) + i);
            idx.push_back((levels - 2) * (slices + 1) + i% (slices + 1) + 1);
            idx.push_back((levels - 1) * (slices + 1) + 1);
        }
    }


    Mesh::MeshData desc = {};
    desc.pVertex = vtx.data();
    desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
    desc.vertexCount = static_cast<UINT>(vtx.size());
    desc.pIndex = idx.data();
    desc.indexSize = sizeof(DWORD);
    desc.indexCount = static_cast<UINT>(idx.size());
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    //desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    mMesh = std::make_unique<Mesh>(desc);

    //頂点情報をmVerticesに渡す
    SetVertices(vtx);
    
}


void Sphere::WriteDefShader()
{
    if (!mDefPS || !mDefVS)
    {
        DebugLog::LogError("ShaderFile is not set");
        return;
    }

    CameraBase* firstCamera = GameApp::GetCurrentCamera();
    DirLight* dirLight = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight").get();

    XMFLOAT4X4 WVP[3] = {};
    //WORLD
    WVP[0] = mTransform.GetMatrixFX4();

    //VIEW
    WVP[1] = firstCamera->GetViewXMF();

    //PROJ
    WVP[2] = firstCamera->GetProjXMF();

    XMFLOAT4 eyePos= { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

	NormalConstantBuffer cb = {

            eyePos,
            Vector4{dirLight->GetAmbient().x,dirLight->GetAmbient().y,dirLight->GetAmbient().z,dirLight->GetAmbient().w},
           Vector4{ dirLight->GetDiffuse().x,dirLight->GetDiffuse().y,dirLight->GetDiffuse().z,dirLight->GetDiffuse().w},
        Vector4{dirLight->GetPosition().x,dirLight->GetPosition().y,dirLight->GetPosition().z,0},
        mMaterial.material,
    };


    UVConstantBuffer uvBuffer;
    uvBuffer.useUV = isUseUVAnimation;
    //UV MATRIX 作成
    if (isUseUVAnimation)
    {
        uvBuffer.uv = XMMatrixTranslation(mUvAnimation->GetOffsetUV().x, mUvAnimation->GetOffsetUV().y, 0.0f);
        uvBuffer.uv = XMMatrixTranspose(uvBuffer.uv);
    }

    mDefVS->WriteShader(0, WVP);
    mDefVS->WriteShader(1, &uvBuffer);
    mDefPS->WriteShader(0, &cb);
}


#include "Sphere.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

Sphere::Sphere() :Primitive(SPHERE)
{
}

void Sphere::Init(const char* _fileName)
{
    CreateMesh(64, 64);
	CreateMaterial();
    CreateTexture(_fileName);
    LoadDefShader();
  
}

void Sphere::Update(float dt)
{

    WriteDefShader();
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

const void Sphere::CreateMesh(UINT levels, UINT slices)
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
                XMFLOAT2(theta / XM_2PI, phi / XM_PI)
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

const void Sphere::CreateMaterial()
{
    mMaterial.material = {
        Color(1.0f, 1.0f, 1.0f, 1.0f),		// 環境光
        Color(1.0f, 1.0f, 1.0f, 1.0f),		// 表面色
        Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
        Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
    };
}

const void Sphere::CreateTexture(const char* fileName)
{
    if (!fileName)
    {
        mMaterial.material.isTexEnable = false;
        mMaterial.tex = nullptr;
        return;
    }

    mMaterial.tex = std::make_unique<Texture>();
    HRESULT hr = mMaterial.tex->Create(fileName);
    if (FAILED(hr))
    {
        mMaterial.tex = nullptr;
        mMaterial.material.isTexEnable = false;
    }
	mFilePath = fileName;
}

const void Sphere::WriteDefShader()
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

    XMFLOAT4 eyePos= { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

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


#include "GampApp.h"
#include <cassert>

#include "d3dUtil.h"
#include "DXTrace.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight):D3DApp(hInstance, windowName, initWidth, initHeight)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;
	
    if (!InitEffect())
        return false;

    if (!InitResource())
        return false;

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
    static float phi = 0.0f, theta = 0.0f;
    phi += 0.3f * dt, theta += 0.37f * dt;
    cb.world = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
    // 更新常量缓冲区，让立方体转起来
   /* D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(m_pd3dImmediateContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));
    m_pd3dImmediateContext->Unmap(m_pConstantBuffer.Get(), 0);*/
}

void GameApp::DrawScene()
{
	assert(mContext);
	assert(mSwapChain);
	static float blue[4] = { 0.2f, 0.3f, 0.4f, 1.0f };  // RGBA = (0,0,255,255)
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), blue);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

   
	mVertexShader->SetShader();
    mVertexShader->WriteShader(0, &cb);
    mPixelShader->SetShader();

    mesh->Draw();

	mSwapChain->Present(0, 0);

}

bool GameApp::InitEffect()
{
    mVertexShader = std::make_shared<VertexShader>(ShaderEnum::Vertex);
    mPixelShader = std::make_shared<PixelShader>(ShaderEnum::Pixel);


    //Create VertexShader
    HR(mVertexShader->LoadShaderFile("Assets/Shader/VS_UliObj.cso"));

	//Create Pixel Shader
    HR(mPixelShader->LoadShaderFile("Assets/Shader/PS_UliObj.cso"));

    return true;
}

bool GameApp::InitResource()
{
    // ******************
    // 设置立方体顶点
    //    5________ 6
    //    /|      /|
    //   /_|_____/ |
    //  1|4|_ _ 2|_|7
    //   | /     | /
    //   |/______|/
    //  0       3

    Vertex vertices[] =
    {
	    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof vertices;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    
    DWORD indices[] = {
        // 正面
        0, 1, 2,
        2, 0, 3,
        // 左面
        4, 5, 1,
        1, 4, 0,
        // 顶面
        1, 5, 6,
        6, 1, 2,
        // 背面
        7, 6, 5,
        5, 7, 4,
        // 右面
        3, 2, 6,
        6, 3, 7,
        // 底面
        4, 0, 3,
        3, 4, 7
    };

    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));

    Mesh::MeshData data={};
    data.pVertex = vertices;
    data.vertexSize = sizeof(Vertex);
    data.vertexCount = _countof(vertices);

	data.pIndex = indices;
    data.indexSize = sizeof(DWORD);
    data.indexCount =_countof(indices);


    data.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    mesh= std::make_unique<Mesh>(data);


    cb.world = XMMatrixIdentity();	// 单位矩阵的转置是它本身
    cb.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    cb.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));

   
    return true;
}

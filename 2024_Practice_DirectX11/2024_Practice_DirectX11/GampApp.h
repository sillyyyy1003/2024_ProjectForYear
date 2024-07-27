#pragma once
#include <DirectXMath.h>
#include <memory>
#include "d3dApp.h"
#include "Mesh.h"
#include "Shader.h"

class GameApp : public D3DApp
{
private:


    std::shared_ptr<VertexShader> mVertexShader;
    std::shared_ptr<PixelShader> mPixelShader;

    ComPtr<ID3D11Buffer> mVertexBuffer;
    std::unique_ptr<Mesh> mesh;

public:
    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
       
    };

    struct ConstantBuffer
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
    };

    ConstantBuffer cb = {};

public:

    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp();

    /// @brief 
    /// @return 
    bool Init();

    /// @brief 
    void OnResize();

    /// @brief
    /// @param dt 
    void UpdateScene(float tick);

    /// @brief 
    void DrawScene();

private:
    bool InitEffect();
    bool InitResource();
};

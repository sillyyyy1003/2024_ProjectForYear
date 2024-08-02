#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Box3D.h"
#include "d3dApp.h"
#include "Geometry.h"
#include "Mesh.h"
#include "Shader.h"

class Box3D;
class GameApp : public D3DApp
{
private:
	/// @brief For Rasterizer State(光栅化)
	ComPtr<ID3D11RasterizerState> mRSWireframe;
    bool isWireframeMode;							


public:
    Box3D box;
public:

    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp() override;

    /// @brief 
    /// @return 
    bool Init() override;

    /// @brief 
    void OnResize() override;

    /// @brief
    /// @param deltaTime tick
    void UpdateScene(float deltaTime) override;

    /// @brief 
    void DrawScene() override;

private:

    bool InitResource();
};

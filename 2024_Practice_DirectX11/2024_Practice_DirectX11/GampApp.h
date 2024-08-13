#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Box3D.h"
#include "d3dApp.h"
#include "FirstPersonCamera.h"
#include "Geometry.h"
#include "Mesh.h"
#include "Plane3D.h"
#include "Shader.h"

class Model;
class Box3D;
class GameApp : public D3DApp
{
private:
	/// @brief For Rasterizer State(光栅化)
	ComPtr<ID3D11RasterizerState> mRSWireframe;
    bool isWireframeMode = false;


public:

    std::unique_ptr<Box3D> box;
    std::shared_ptr<Plane3D> bg[3];
    std::shared_ptr<Model> model;

    std::unique_ptr<FirstPersonCamera> firstCamera;

    std::shared_ptr<VertexShader> VS;
    std::shared_ptr<PixelShader> PS;
 
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

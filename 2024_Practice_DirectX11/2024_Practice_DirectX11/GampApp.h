#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Box3D.h"
#include "d3dApp.h"
#include "FirstPersonCamera.h"
#include "Geometry.h"
#include "Plane3D.h"
#include "Shader.h"

class Model;
class Box3D;
class SceneManager;
class GameApp : public D3DApp
{
private:
	/// @brief For Rasterizer State
	ComPtr<ID3D11RasterizerState> mRSWireframe;
    bool isWireframeMode = false;

    ComPtr<ID2D1SolidColorBrush> mpColorBrush;  // Brush
    ComPtr<IDWriteFont> mpFont;					// Font
    ComPtr<IDWriteTextFormat> mpTextFormat;		// Text Format

public:
    std::shared_ptr<SceneManager> mSceneManager;

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

    void UnInit() override;

private:

    bool InitResource();

};

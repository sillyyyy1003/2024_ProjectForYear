#pragma once
#include <memory>
#include "d3dApp.h"
#include "Geometry.h"
#include "SceneManager.h"
#include "UI2D.h"

class GameApp : public D3DApp
{
private:
	/// @brief For Rasterizer State
	ComPtr<ID3D11RasterizerState> mRSWireframe;
    bool isWireframeMode = false;

public:
    ComPtr<ID2D1SolidColorBrush> mpColorBrush;  // Brush
    ComPtr<IDWriteTextFormat> mpTextFormat;		// Text Format

public:

    std::unique_ptr<SceneManager> mSceneManager = nullptr;
    std::unique_ptr<UI2D> ui = nullptr;
private:
    GameApp(){};
    ~GameApp();

public:

    static GameApp* Get(){ static GameApp instance; return &instance; }

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

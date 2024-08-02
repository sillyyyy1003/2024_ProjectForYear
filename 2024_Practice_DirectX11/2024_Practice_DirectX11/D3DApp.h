#pragma once

#include "D3DUtil.h"
#include "Timer.h"
#include <wrl/client.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>


using namespace Microsoft::WRL;

class Timer;
class D3DApp;

extern D3DApp* gD3D;

class D3DApp
{
protected:

    HINSTANCE mhAppInst;
    HWND      mhMainWnd;
    bool      isAppPaused = false;       // Pause or not
    bool      isMinimized = false;
    bool      isMaximized = false;
    bool      isResizing = false;        // Application Window change size or not
    
    bool      isEnable4xMsaa = true;    // is Use Multiple Anti Aliasing
    UINT      m4xMsaaQuality = 0;       //  Multiple Anti Aliasing
    

    // Direct3D 11
    ComPtr<ID3D11Device> mDevice = nullptr;
    ComPtr<ID3D11DeviceContext> mContext = nullptr;
    ComPtr<IDXGISwapChain> mSwapChain = nullptr;

    //Use in HIGH LEVEL RENDERING
    ComPtr<ID3D11Texture2D> mDepthStencilBuffer = nullptr;
    ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
    ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
    D3D11_VIEWPORT mViewport;

    std::wstring mWndTitle; //Window Name                 
    int mClientWidth = 0;       //Client Width
    int mClientHeight = 0;      //Client Height

    Timer mTimer;

public:
    D3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    virtual ~D3DApp();


    ID3D11Device* GetDevice() const { return mDevice.Get(); };
    ID3D11DeviceContext* GetContext() const { return mContext.Get(); };

    HINSTANCE AppInst()const { return mhAppInst; };   // 初期化
    HWND      MainWnd()const { return mhMainWnd; };   // Get Main Wnd 
    float     AspectRatio()const;                     // Calculate Aspect Ratio

    int Run();                                  // Run Application

    /// @brief アプリの初期化
    /// @return 
    virtual bool Init();

    /// @brief Wnd Size調整
    virtual void OnResize();

    /// @brief 
    /// @param tick 
    virtual void UpdateScene(float tick);

    /// @brief 描画
    virtual void DrawScene();

    /// @brief Wnd Message
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


protected:
    /// @brief Init Main Window
    /// @return 
    bool InitMainWindow();

    /// @brief Init D3D
    /// @return 
    bool InitDirect3D();

    /// @brief Init imGui
    /// @return 
    bool InitImGui();            

    /// @brief フレーム数を計算＆表示
    void CalculateFrameStats();  

};


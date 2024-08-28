#pragma once

#include "D3DUtil.h"
#include "Timer.h"
#include <wrl/client.h>
#include <d2d1.h>
#include <dwrite.h>


using namespace Microsoft::WRL;

class Timer;
class D3DApp;

extern D3DApp* gD3D;
namespace D3D
{
    enum SamplerState
    {
        SAMPLER_LINEAR,
        SAMPLER_POINT,
        SAMPLER_MAX
    };

    enum BlendState
    {
        BLEND_NONE,
        BLEND_ALPHA,
        BLEND_ADD,
        BLEND_ADDALPHA,
        BLEND_SUB,
        BLEND_SCREEN,
        BLEND_MULTI,
        BLEND_MAX
    };

}

class D3DApp
{
public:

protected:

    HINSTANCE mhAppInst={};
    HWND      mhMainWnd={};
    bool      isAppPaused = false;       // Pause or not
    bool      isMinimized = false;
    bool      isMaximized = false;
    bool      isResizing = false;        // Application Window change size or not
    
    bool      isEnable4xMsaa = true;    // is Use Multiple Anti Aliasing
    UINT      m4xMsaaQuality = 0;       //  Multiple Anti Aliasing
    

    
    // Direct2D 
    ComPtr<ID2D1Factory> mpD2DFactory;			    // D2DFactory
    ComPtr<ID2D1RenderTarget> mpD2DRenderTarget;    // D2DRenderTarget
    ComPtr<IDWriteFactory> mpDWriteFactory;			// DWrite Factory

    // Direct3D 11
    ComPtr<ID3D11Device> mDevice = nullptr;
    ComPtr<ID3D11DeviceContext> mContext = nullptr;
    ComPtr<IDXGISwapChain> mSwapChain = nullptr;

    //Use in HIGH LEVEL RENDERING
    ComPtr<ID3D11Texture2D> mDepthStencilBuffer = nullptr;
    ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
    ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
    D3D11_VIEWPORT mViewport = {};

    ComPtr<ID3D11SamplerState> mSamplerState[D3D::SAMPLER_MAX] = {};
    ComPtr<ID3D11BlendState> mBlendStates[D3D::BLEND_MAX] = {};

    std::wstring mWndTitle={}; //Window Name                 
    int mClientWidth = 0;       //Client Width
    int mClientHeight = 0;      //Client Height

    Timer mTimer;
    FILE* fp = nullptr;
   
public:

	D3DApp() {};
    virtual ~D3DApp();

    void InitDX(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	virtual void UnInit();


    ID3D11Device* GetDevice() const { return mDevice.Get(); };
    ID3D11DeviceContext* GetContext() const { return mContext.Get(); };
    IDXGISwapChain* GetSwapChain() const { return mSwapChain.Get(); };

    ID2D1RenderTarget* Get2DRenderTarget() const { return mpD2DRenderTarget.Get(); };
    ID2D1Factory* GetD2DFactory()const { return mpD2DFactory.Get(); };
    IDWriteFactory* GetWriteFactory()const { return mpDWriteFactory.Get(); };

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

    int GetWinWidth() { return mClientWidth; };
    int GetWinHeight() { return mClientHeight; };

protected:
    /// @brief Init Main Window
    /// @return succeed or not
    bool InitMainWindow();

    /// @brief Init D2D
    /// @return succeed or not
    bool InitDirect2D();

    /// @brief Init D3D
    /// @return succeed or not
    bool InitDirect3D();

    /// @brief Init imGui
    /// @return succeed or not
    bool InitImGui();            

    /// @brief フレーム数を計算＆表示
    void CalculateFrameStats();

    /// @brief サンプラーステート設定
    /// @param _state 
    void SetSamplerState(D3D::SamplerState _state);

    /// @brief ブレンドステートを設定
    /// @param _state ブレンドステート名
    void SetBlendState(D3D::BlendState _state);

};


#pragma once
#include "D3DUtil.h"
#include "Timer.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Texture.h"

using namespace Microsoft::WRL;

class D3DApp;
extern D3DApp* gD3D;

class D3DApp
{

  
protected:

    HINSTANCE mhAppInst = {};
    HWND      mhMainWnd = {};
    bool      isAppPaused = false;       // Pause or not
    bool      isMinimized = false;
    bool      isMaximized = false;
    bool      isResizing = false;        // Application Window change size or not
    bool      isResized = false;         // Trigger for UI to ChangeSize 

    bool      isEnable4xMsaa = false;    // is Use Multiple Anti Aliasing
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
   
    std::shared_ptr<RenderTarget> mRenderTarget = nullptr;
    std::shared_ptr<DepthStencil> mDepthStencil = nullptr;

    D3D11_VIEWPORT mViewport = {};

    std::wstring mWndTitle = {}; //Window Name                 
    int mClientWidth = 0;       //Client Width
    int mClientHeight = 0;      //Client Height

    Timer mTimer;

    FILE* fp = nullptr;

    //Scroll wheelによる操作
    int mScrollValue = 0;	// WIN_APIを使う
    int moveUnit = 0;		// wheelによる移動量
   
public:

	D3DApp() {};
    virtual ~D3DApp();

    void InitDX(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	virtual void UnInit();


    ID3D11Device* GetDevice() const { return mDevice.Get(); };
    ID3D11DeviceContext* GetContext() const { return mContext.Get(); };
    IDXGISwapChain* GetSwapChain() const { return mSwapChain.Get(); };

    RenderTarget* GetDefaultRTV() const { return mRenderTarget.get(); };
    DepthStencil* GetDefaultDSV() const { return mDepthStencil.get(); };

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

	int GetWinWidth();
    int GetWinHeight();;

    /// @brief サンプラーステート設定
	/// @param _state 
    static void SetSamplerState(ComPtr<ID3D11SamplerState> _state);

    /// @brief ブレンドステートを設定
    /// @param _state ブレンドステート名
    static void SetBlendState(ComPtr<ID3D11BlendState> _state);

    /// @brief Set Rasterizer State
    /// @param _rsState 
    static void SetCullingMode(ComPtr<ID3D11RasterizerState> _rsState);

    static void SetDepthTest(ComPtr<ID3D11DepthStencilState> _state);

    static void SetRenderTarget(UINT num, RenderTarget** ppViews, DepthStencil* pView);
    static void SetRenderTarget(UINT num, std::shared_ptr<RenderTarget>* ppViews, DepthStencil* pView);
    static void SetDefaultRenderTarget();

    bool GetResized() { return isResized; };

    //Wheel Unit
    int GetMoveUnit() { return moveUnit; };
    void SetMoveUnit(int moveUnit) { this->moveUnit = moveUnit; };

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


};


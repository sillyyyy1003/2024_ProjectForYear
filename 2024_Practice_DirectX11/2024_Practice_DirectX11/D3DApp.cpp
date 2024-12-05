#include "d3dApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include <sstream>
#include <dxgi.h>
#include <memory>

#include "DebugLog.h"
#include "GameApp.h"
#include "KInput.h"
#include "RenderState.h"

#pragma warning(disable: 6031)
#pragma warning(disable: 4099)

D3DApp* gD3D = nullptr;

extern "C"
{
   //AMD&Nvidia優先使用
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x00000001;
}

namespace
{
    // This is just used to forward Windows messages from a global window
    // procedure to our member function window procedure because we cannot
    // assign a member function to WNDCLASS::lpfnWndProc.
    //D3DApp* gD3D = nullptr;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Forward hWnd on because we can get messages (e.g., WmCREATE)
    // before CreateWindow returns, and thus before mhMainWnd is valid.
    return GameApp::Get()->MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp::~D3DApp()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void D3DApp::InitDX(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
{
    mhAppInst = hInstance;
    mWndTitle = windowName;
    mClientWidth = initWidth;
    mClientHeight = initHeight;
    ZeroMemory(&mViewport, sizeof(D3D11_VIEWPORT));
	gD3D = this;
}

void D3DApp::UnInit()
{
    Microsoft::WRL::ComPtr<ID3D11InfoQueue> infoQueue;
    if (SUCCEEDED(gD3D->GetDevice()->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(infoQueue.GetAddressOf()))))
    {
        infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
    }


    if (mContext)
    {
        mContext->ClearState();
    }
}


float D3DApp::AspectRatio()const
{
    return static_cast<float>(mClientWidth) / mClientHeight;
}

int D3DApp::Run()
{

    MSG msg = {};
    mTimer.Reset();
    timeBeginPeriod(1);
    mTimer.mOldTime = timeGetTime();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            mTimer.Tick();

            if (!isAppPaused)
            {
                mTimer.mNewTime = timeGetTime();
                float diff = static_cast<float>(mTimer.mNewTime - mTimer.mOldTime);
                if (diff >= 1000.0f / 60)
                {
                    diff /= 1000;//transfer second to milliSec
                    CalculateFrameStats();
                    ImGui_ImplDX11_NewFrame();
                    ImGui_ImplWin32_NewFrame();
                    ImGui::NewFrame();
                    KInput::UpdateInput();//入力
                    UpdateScene(diff);
                    DrawScene();
                    mTimer.mOldTime = mTimer.mNewTime;
                }          
                /*
                    CalculateFrameStats();
	               ImGui_ImplDX11_NewFrame();
	               ImGui_ImplWin32_NewFrame();
	               ImGui::NewFrame();
	               KInput::UpdateInput();//入力
	               UpdateScene(mTimer.DeltaTime());
	               DrawScene();
                */
            }
            else
            {
                Sleep(100);
            }
        }
    }
    timeEndPeriod(1);
    UnInit();
    fclose(fp);
    return (int)msg.wParam;
}

bool D3DApp::Init()
{
    if (!InitMainWindow())
        return false;

    if (!InitDirect2D())
        return false;

    if (!InitDirect3D())
        return false;

    if (FAILED(KInput::InitInput()))
        return false;

	if (!InitImGui())
        return false;

    // コンソール生成
    AllocConsole();
    // 標準出力の割り当て
    freopen_s(&fp, "CON", "w", stdout);

    DebugLog::Log("Game Init Completed");
    return true;
}

void D3DApp::OnResize()
{
    assert(mContext);
    assert(mDevice);
    assert(mSwapChain);

    //RESET RESOURCE
    mRenderTargetView.Reset();
	mDepthStencilView.Reset();
    mDepthStencilBuffer.Reset();

    mRenderTarget.reset();
    mDepthStencil.reset();
    mRenderTarget = std::make_shared<RenderTarget>();
    mDepthStencil = std::make_shared<DepthStencil>();

    // 重设交换链并且重新创建渲染目标视图
    ComPtr<ID3D11Texture2D> backBuffer;
    HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0));	// 注意此处DXGI_FORMAT_B8G8R8A8_UNORM
    HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
    HR(mRenderTarget->CreateFromScreen());

    // Set Debug Name
    D3D11SetDebugObjectName(backBuffer.Get(), "BackBuffer[0]");
    backBuffer.Reset();


    //Set Depth Stencil
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = mClientWidth;
    depthStencilDesc.Height = mClientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // if use 4X MSAA
    if (isEnable4xMsaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }
	
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    // Create DepthStencilView & depth stencil buffer
    HR(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));
    HR(mDepthStencil->Create(gD3D->GetWinWidth(),gD3D->GetWinHeight(),false));

    //Set Default Render Target
    SetRenderTarget(1, &mRenderTarget, mDepthStencil.get());

}

void D3DApp::UpdateScene(float tick)
{
}

void D3DApp::DrawScene()
{
}

LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(mhMainWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
        // WmACTIVATE is sent when the window is activated or deactivated.  
        // We pause the game when the window is deactivated and unpause it 
        // when it becomes active.  
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            isAppPaused = true;
            mTimer.Stop();
        }
        else
        {
            isAppPaused = false;
            mTimer.Start();
        }
        return 0;

        // WmSIZE is sent when the user resizes the window.  
    case WM_SIZE:
     
        // Save the new client area dimensions.
        mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
        if (mDevice)
        {
            if (wParam == SIZE_MINIMIZED)
            {
                isAppPaused = true;
                isMinimized = true;
                isMaximized = false;
            }
            else if (wParam == SIZE_MAXIMIZED)
            {
                isAppPaused = false;
                isMinimized = false;
				isMaximized = true;
                OnResize();
            }
            else if (wParam == SIZE_RESTORED)
            {

                // Restoring from minimized state?
                if (isMinimized)
                {
                    isAppPaused = false;
                	isMinimized = false;
                    OnResize();
                }

                // Restoring from maximized state?
                else if (isMaximized)
                {
                    isAppPaused = false;
                	isMaximized = false;
                    OnResize();
                }
                else if (isResizing)
                {
                    // If user is dragging the resize bars, we do not resize 
                    // the buffers here because as the user continuously 
                    // drags the resize bars, a stream of WmSIZE messages are
                    // sent to the window, and it would be pointless (and slow)
                    // to resize for each WmSIZE message received from dragging
                    // the resize bars.  So instead, we reset after the user is 
                    // done resizing the window and releases the resize bars, which 
                    // sends a WmEXITSIZEMOVE message.
                }
                else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
                {
                    OnResize();
                }
            }
        }

        return 0;

        // WmEXITSIZEMOVE is sent when the user grabs the resize bars.
    case WM_ENTERSIZEMOVE:
        isAppPaused = true;
        isResizing = true;
        mTimer.Stop();
        return 0;

        // WmEXITSIZEMOVE is sent when the user releases the resize bars.
        // Here we reset everything based on the new window dimensions.
    case WM_EXITSIZEMOVE:
        isAppPaused = false;
        isResizing = false;
        mTimer.Start();
        OnResize();
        return 0;

        // WmDESTROY is sent when the window is being destroyed.
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        // The WmMENUCHAR message is sent when a menu is active and the user presses 
        // a key that does not correspond to any mnemonic or accelerator key. 
    case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);

        // Catch this message so to prevent the window from becoming too small.
    case WM_GETMINMAXINFO:
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
        return 0;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        return 0;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        return 0;
    case WM_MOUSEMOVE:
        return 0;
    case WM_MOUSEWHEEL:
        mScrollValue = GET_WHEEL_DELTA_WPARAM(wParam);
        moveUnit = mScrollValue / 120;
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int D3DApp::GetWinWidth()
{
    if (mClientWidth <= 0)
        return 1;
	return mClientWidth;
}

int D3DApp::GetWinHeight()
{
    if (mClientHeight <= 0)
        return 1;
	return mClientHeight;
}


bool D3DApp::InitMainWindow()
{
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = mhAppInst;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = L"D3DWndClassName";

    if (!RegisterClass(&wc))
    {
        MessageBox(0, L"RegisterClass Failed.", 0, 0);
        return false;
    }

    // Compute window rectangle dimensions based on requested client area dimensions.
    RECT R = { 0, 0, mClientWidth, mClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    int width = R.right - R.left;
    int height = R.bottom - R.top;

    mhMainWnd = CreateWindow(L"D3DWndClassName", mWndTitle.c_str(),WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0);

  
    if (!mhMainWnd)
    {
        MessageBox(0, L"CreateWindow Failed.", 0, 0);
        return false;
    }

    ShowWindow(mhMainWnd, SW_SHOW);
    UpdateWindow(mhMainWnd);

    return true;
}

bool D3DApp::InitDirect2D()
{

    HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, mpD2DFactory.GetAddressOf()));
    HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),reinterpret_cast<IUnknown**>(mpDWriteFactory.GetAddressOf())));

    return true;
}

bool D3DApp::InitDirect3D()
{

	HRESULT hr = S_OK;

// 创建D3D设备 和 D3D设备上下文
	UINT createDeviceFlags = 0;
	#ifndef USE_IMGUI
	createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// Direct2D需要支持BGRA格式
	#endif
	#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
// 驱动类型数组
	D3D_DRIVER_TYPE driverTypes[] =
	{
	    D3D_DRIVER_TYPE_HARDWARE,
	    D3D_DRIVER_TYPE_WARP,
	    D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// 特性等级数组
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	    D3D_FEATURE_LEVEL_11_1,
	    D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
	    d3dDriverType = driverTypes[driverTypeIndex];
	    hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
	        D3D11_SDK_VERSION, mDevice.GetAddressOf(), &featureLevel, mContext.GetAddressOf());

	    if (hr == E_INVALIDARG)
	    {
	        // Direct3D 11.0 的API不承认D3D_FEATURE_LEVEL_11_1，所以我们需要尝试特性等级11.0以及以下的版本
	        hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
	            D3D11_SDK_VERSION, mDevice.GetAddressOf(), &featureLevel, mContext.GetAddressOf());
	    }

	    if (SUCCEEDED(hr))
	        break;
	}

	if (FAILED(hr))
	{
	    MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
	    return false;
	}

	// 检测是否支持特性等级11.0或11.1
	if (featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
	{
	    MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
	    return false;
	}

    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
    ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(包含DXGI1.1)的接口类
    ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(包含DXGI1.2)特有的接口类

    // 为了正确创建 DXGI交换链，首先我们需要获取创建 D3D设备 的 DXGI工厂，否则会引发报错：
    // "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
    HR(mDevice.As(&dxgiDevice));
    HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
    HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));

	// 填充DXGI_SWAP_CHAIN_DESC用以描述交换链
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = mClientWidth;
    sd.BufferDesc.Height = mClientHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//If is Enable 4xMsaa
    if (isEnable4xMsaa)
    {
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = mhMainWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
    HR(dxgiFactory1->CreateSwapChain(mDevice.Get(), &sd, mSwapChain.GetAddressOf()));
    

    // 可以禁止alt+enter全屏
    dxgiFactory1->MakeWindowAssociation(mhMainWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);



    // Set Debug Obj
    D3D11SetDebugObjectName(mContext.Get(), "ImmediateContext");
    DXGISetDebugObjectName(mSwapChain.Get(), "SwapChain");

    //******************************************
	// Init Render State
	//******************************************
    RenderState::InitAll(mDevice.Get());
    //Set Sampler state
    SetSamplerState(RenderState::SSLinearWrap);
    //Set Blend state
    SetBlendState(RenderState::BSTransparent);
    //Set Rasterizer state
    SetCullingMode(RenderState::RSNoCull);

    //Init Resize Tool
    OnResize();

    return true;
}

bool D3DApp::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // 允许键盘控制
    io.ConfigWindowsMoveFromTitleBarOnly = true;              // 仅允许标题拖动

    // SET Dear ImGui Style
    ImGui::StyleColorsDark();

    // Set Platform/Render back End
    ImGui_ImplWin32_Init(mhMainWnd);
    ImGui_ImplDX11_Init(mDevice.Get(), mContext.Get());

    return true;
}

void D3DApp::CalculateFrameStats()
{
    // 毎秒のフレーム数とフレームレンダリングの時間
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;
    //1s単位で計算する
    if ((mTimer.TotalTime() - timeElapsed) >= 1.f)
    {
        float fps = (float)frameCnt; // fps = frameCnt / 1
        float mspf = 1000.0f / fps;

        std::wostringstream outs;
        outs.precision(6);
        outs << mWndTitle << L"    "
            << L"FPS: " << fps << L"    "
            << L"Frame Time: " << mspf << L" (ms)";
        SetWindowText(mhMainWnd, outs.str().c_str());

        // Reset for next average.
        frameCnt = 0;
        timeElapsed += 1.0f;
    }
}

void D3DApp::SetSamplerState(ComPtr<ID3D11SamplerState> _state)
{
    gD3D->GetContext()->VSSetSamplers(0, 1, _state.GetAddressOf());
    gD3D->GetContext()->PSSetSamplers(0, 1, _state.GetAddressOf());
    gD3D->GetContext()->HSSetSamplers(0, 1, _state.GetAddressOf());
    gD3D->GetContext()->DSSetSamplers(0, 1, _state.GetAddressOf());
}

void D3DApp::SetBlendState(ComPtr<ID3D11BlendState> _state)
{
    gD3D->GetContext()->OMSetBlendState(_state.Get(), nullptr, 0xffffffff);
}

void D3DApp::SetCullingMode(ComPtr<ID3D11RasterizerState> _rsState)
{
    gD3D->GetContext()->RSSetState(_rsState.Get());
}

void D3DApp::SetDepthTest(ComPtr<ID3D11DepthStencilState> _state)
{
    gD3D->GetContext()->OMSetDepthStencilState(_state.Get(), 0);
}

void D3DApp::SetRenderTarget(UINT num, RenderTarget** ppViews, DepthStencil* pView)
{
    static ID3D11RenderTargetView* rtvs[4];

    if (num > 4) num = 4;
    for (UINT i = 0; i < num; ++i)
        rtvs[i] = ppViews[i]->GetView();
    gD3D->mContext->OMSetRenderTargets(num, rtvs, pView ? pView->GetView() : nullptr);

    // ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = (float)ppViews[0]->GetWidth();
    vp.Height = (float)ppViews[0]->GetHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    gD3D->mContext->RSSetViewports(1, &vp);
}

void D3DApp::SetRenderTarget(UINT num, std::shared_ptr<RenderTarget>* ppViews, DepthStencil* pView)
{
    static ID3D11RenderTargetView* rtvs[4];

    if (num > 4) num = 4;
    for (UINT i = 0; i < num; ++i)
        rtvs[i] = ppViews[i]->GetView();
    gD3D->mContext->OMSetRenderTargets(num, rtvs, pView ? pView->GetView() : nullptr);

    // ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = (float)ppViews[0]->GetWidth();
    vp.Height = (float)ppViews[0]->GetHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    gD3D->mContext->RSSetViewports(1, &vp);
}


void D3DApp::SetDefaultRenderTarget()
{
    SetRenderTarget(1, &gD3D->mRenderTarget, gD3D->mDepthStencil.get());
}


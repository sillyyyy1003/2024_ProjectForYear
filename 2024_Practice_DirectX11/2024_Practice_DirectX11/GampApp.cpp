#include "GampApp.h"
#include <cassert>
#include "Model.h"
#include "d3dUtil.h"
#include "GUI.h"
#include "KInput.h"
#include "SceneManager.h"

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

    if (!InitResource())
        return false;

	return true;
}

void GameApp::OnResize()
{
	
	assert(mpD2DFactory);
	assert(mpDWriteFactory);

	// リリースD2D
	mpColorBrush.Reset();
	mpD2DRenderTarget.Reset();

	D3DApp::OnResize();

	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = mpD2DFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, mpD2DRenderTarget.GetAddressOf());
	surface.Reset();

	 if (hr == S_OK)
	{
		// CreateBrush
		HR(mpD2DRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			mpColorBrush.GetAddressOf()));
		HR(mpDWriteFactory->CreateTextFormat(
			L"Meiryo",      // Font family name
			nullptr,        // Font collection (nullptr for system default)
			DWRITE_FONT_WEIGHT_NORMAL,	// Font weight (e.g., Normal, Bold)
			DWRITE_FONT_STYLE_NORMAL,   // Font style (e.g., Normal, Italic)
			DWRITE_FONT_STRETCH_NORMAL, // Font stretch (e.g., Normal, Condensed)
			20,                         // Font size
			L"ja-JP",                   // Locale (e.g., en-us for English, zh-cn for Simplified Chinese)
			mpTextFormat.GetAddressOf()// Address of the text format pointer
		));
	}
	else
	{
		assert(mpD2DRenderTarget);
	}

}

void GameApp::UpdateScene(float deltaTime)
{

	mSceneManager->_update(deltaTime);
	ImGui::End();
	ImGui::Render();

	
}

void GameApp::DrawScene()
{
	assert(mContext);
	assert(mSwapChain);
	static float blue[4] = { 0.2f, 0.3f, 0.4f, 1.0f };  // RGBA = (0,0,255,255)
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), blue);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mSceneManager->_draw();

	//D2D描画
	if (mpD2DRenderTarget != nullptr)
	{
		mpD2DRenderTarget->BeginDraw();
		std::wstring textStr = L"日本語対応\n"
			L"これはひらかなです\n"
			L"コレハカタカナデス";
		if (isWireframeMode)
			textStr += L"线框模式";
		else
			textStr += L"面模式";
		mpD2DRenderTarget->DrawTextW(textStr.c_str(), (UINT32)textStr.size(), mpTextFormat.Get(),
			D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mpColorBrush.Get());
		HR(mpD2DRenderTarget->EndDraw());
	}

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	mSwapChain->Present(0, 0);

}

void GameApp::UnInit()
{
	//Geometry Release
	Geometry::UnInit();

	//Scene Release
	mSceneManager->UnInit();

	D3DApp::UnInit();
}

bool GameApp::InitResource()
{
	mSceneManager = std::make_shared<SceneManager>();
	mSceneManager->Init();

	// ******************
	// Init Rasterize state
	// ******************
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(mDevice->CreateRasterizerState(&rasterizerDesc, mRSWireframe.GetAddressOf()));

	return true;
}


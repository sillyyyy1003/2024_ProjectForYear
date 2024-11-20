#include "GampApp.h"
#include "Model.h"
#include "d3dUtil.h"
#include "GUI.h"
#include "KInput.h"
#include "SceneManager.h"
#include "Sprite.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


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

	//ResetD2D RenderTarget
	mpD2DRenderTarget.Reset();

	// リリースD2
	D3DApp::OnResize();
	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HR(mpD2DFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, mpD2DRenderTarget.GetAddressOf()));
	surface.Reset();

	//UIの変更を指示する
	isResized = true;

}

void GameApp::UpdateScene(float deltaTime)
{
	//Game update
	SceneManager::Get()->_update(deltaTime);

	//すべてのUI変更終わったあと　Switch Off
	if (isResized)
	{
		isResized = false;
	}

	ImGui::Render();
}

void GameApp::DrawScene()
{
	assert(mContext);
	assert(mSwapChain);
	static float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };  // RGBA = (0,0,255,255)
	mContext->ClearDepthStencilView(mDepthStencil->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	mContext->ClearRenderTargetView(mRenderTarget->GetView(), color);

	//Game描画
	SceneManager::Get()->_draw();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	mSwapChain->Present(0, 0);

}

void GameApp::UnInit()
{
	//Scene Release
	SceneManager::Get()->UnInit();

	D3DApp::UnInit();

	Sprite::UnInit();
}

bool GameApp::InitResource()
{
	//シーン管理の初期化を行う
	SceneManager::Get()->Init();

	Sprite::Init();

	return true;
}




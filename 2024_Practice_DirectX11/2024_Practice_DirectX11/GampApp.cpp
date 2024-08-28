#include "GampApp.h"
#include <cassert>
#include "Model.h"
#include "d3dUtil.h"
#include "GUI.h"
#include "KInput.h"
#include "SceneManager.h"

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

	// リリースD2D
	mpColorBrush.Reset();
	mpD2DRenderTarget.Reset();

	D3DApp::OnResize();

	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HR(mpD2DFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, mpD2DRenderTarget.GetAddressOf()));
	surface.Reset();
}

void GameApp::UpdateScene(float deltaTime)
{
	if(ImGui::Begin("Test Text")){}

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

	//D3D描画
	mSceneManager->_draw();

	//D2D描画
	std::string str = mTimer.GetSystemTime();
	ui->DrawTextStr(str, SOLID);

	

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
	mSceneManager = std::make_unique<SceneManager>();
	mSceneManager->Init();

	ui = std::make_unique<UI2D>();
	ui->InitUI2D();

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


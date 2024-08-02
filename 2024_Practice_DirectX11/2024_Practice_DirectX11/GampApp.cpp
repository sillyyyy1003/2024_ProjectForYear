#include "GampApp.h"
#include <cassert>
#include "d3dUtil.h"
#include "KInput.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight):D3DApp(hInstance, windowName, initWidth, initHeight)
{
}

GameApp::~GameApp()
{
    Geometry::UnInit();
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

    Geometry::Init();

    if (!InitResource())
        return false;



	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float deltaTime)
{
    static float phi = 0.0f, theta = 0.0f;
    phi += 0.1f, theta += 0.1f;

	box.mTransform.mRotation.x = phi;
	box.mTransform.mRotation.y = theta;

	
	//ImGui::End();
	//ImGui::Render();
}

void GameApp::DrawScene()
{
	assert(mContext);
	assert(mSwapChain);
	static float blue[4] = { 0.2f, 0.3f, 0.4f, 1.0f };  // RGBA = (0,0,255,255)
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), blue);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	box.Draw();

	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	mSwapChain->Present(0, 0);

}

bool GameApp::InitResource()
{
	//Init Obj
	box.InitResource();
	
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

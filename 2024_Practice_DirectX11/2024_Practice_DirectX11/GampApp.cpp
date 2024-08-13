#include "GampApp.h"
#include <cassert>
#include "Model.h"
#include "d3dUtil.h"
#include "GUI.h"
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

	
	if (ImGui::Begin("WindowsName"))
	{
		//ImGui::Checkbox("RotateSet", &isRotate);
	
	}

	

	ImGui::End();
	ImGui::Render();

	box->Update();
	firstCamera->Update(deltaTime);
}

void GameApp::DrawScene()
{
	assert(mContext);
	assert(mSwapChain);
	static float blue[4] = { 0.2f, 0.3f, 0.4f, 1.0f };  // RGBA = (0,0,255,255)
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), blue);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Material material = box->GetMaterial()[0];

	XMFLOAT4X4 worldMat = model->mTransform.GetMatrix();
	VS->WriteShader(0, &worldMat);

	//Set Camera matrix
	XMFLOAT4X4 cameraMat[2];
	cameraMat[0] = firstCamera->GetViewXMF();
	cameraMat[1] = firstCamera->GetProjXMF();
	VS->WriteShader(1, cameraMat);

	XMFLOAT4 eyePos = { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };
	struct Light
	{
		DirectX::XMFLOAT4 lightAmbient;
		DirectX::XMFLOAT4 lightDiffuse;
		DirectX::XMFLOAT4 lightDir;
	};
	Light light = {
		Color{0.5f,0.5f,0.5f,1.0f},
		Color{0.2f,0.4f,0.6f,1.0f},
		Vector4{0.5,0.5,-5.0f,1.0f},
	};

	PS->WriteShader(1, &eyePos);
	PS->WriteShader(2, &light);
	model->SetPixelShader(PS);
	model->SetVertexShader(VS);
	model->Draw();


	for (int i = 0; i < 3; i++)
	{
		XMFLOAT4X4 bgMat = bg[i]->mTransform.GetMatrix();
		VS->WriteShader(0, &bgMat);
		VS->WriteShader(1, cameraMat);
		PS->WriteShader(0, &material);
		PS->WriteShader(1, &eyePos);
		PS->WriteShader(2, &light);
		bg[i]->SetPixelShader(PS);
		bg[i]->SetVertexShader(VS);
		bg[i]->Draw();

	}


	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	mSwapChain->Present(0, 0);

}

bool GameApp::InitResource()
{
	//Init Obj
	box = std::make_unique<Box3D>();
	box->InitResource("Assets/Texture/field003.jpg");
	box->mTransform.SetScale(1, 1, 1);

	for (int i = 0; i < 3; i++)
	{
		bg[i] = std::make_shared<Plane3D>();
		bg[i]->InitResource("Assets/Texture/wall000.jpg");
		bg[i]->mTransform.SetScale(1, 1.0, 1);

	}

	bg[0]->mTransform.SetPosition(-2, 0, 0);
	bg[0]->mTransform.SetRotationInDegree(-90, 0, 0);

	bg[1]->mTransform.SetPosition(0, -1, 0);

	bg[2]->mTransform.SetPosition(2, -2, 1);
	bg[2]->mTransform.SetRotationInDegree(0, 0, -90);


	model = std::make_shared<Model>();
	model->Load("Assets/Model/Player.obj");
	model->mTransform.SetPosition(1.0f, 1.0f, 0.0f);

	VS = std::make_shared<VertexShader>(ShaderEnum::Vertex);
	HR(VS->LoadShaderFile("Assets/Shader/VS_Box.cso"));

	PS = std::make_shared<PixelShader>(ShaderEnum::Pixel);
	HR(PS->LoadShaderFile("Assets/Shader/PS_Box.cso"));

	firstCamera = std::make_unique<FirstPersonCamera>();
	firstCamera->SetPosition(0.0f, 1.0f, -5.0f);
	
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


#include "SceneBlank.h"
#include <memory>
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "Geometry.h"
#include "Object.h"
#include "Plane3D.h"
#include "RenderState.h"


using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneBlank::Init()
{
	Object* obj = CreateObj<Object>("Object");
	obj->Init(SPHERE,"Assets/Texture/uv-grid-tester_base1.png");


	Material mat={
	Color(0.5f, 0.5f, 0.5f, 1.0f),
	Color(1.0f, 1.0f, 1.0f, 0.5f),
	Color(0.8f, 0.8f, 0.8f, 32.0f),
	Color(0.0f, 0.0f, 0.0f, 0.0f)
	};
	primitive = std::make_unique<Plane3D>();
	primitive->SetTexUV({ 10,10 });
	primitive->Init("Assets/Texture/water.png");
	primitive->SetScale({ 10,10 });
	primitive->SetMaterial(mat);

	floor = std::make_unique<Plane3D>();
	floor->SetTexUV({ 2,2 });
	floor->Init("Assets/Texture/square_tiles_02_diff_1k.jpg");
	floor->SetScale({ 10,10 });
	floor->mTransform.SetPosition(0, -0.5f, 0);


	uiBg = std::make_unique<CanvasUI>();
	uiBg->Init("Assets/Texture/UI/main_title_background_1920x1080.png");

	uiStart = std::make_unique<CanvasUI>();
	uiStart->Init("Assets/Texture/UI/main_title_background_1920x1080.png");
	

	uiOption = std::make_unique<CanvasUI>();
	uiOption->Init("Assets/Texture/UI/main_title_background_1920x1080.png");

	uiExit = std::make_unique<CanvasUI>();
	uiExit->Init("Assets/Texture/UI/main_title_background_1920x1080.png");

}

void SceneBlank::UnInit()
{
	
}

void SceneBlank::Update(float dt)
{
	FirstPersonCamera* camera = GetObj<FirstPersonCamera>("Camera");
	Object* obj = GetObj<Object>("Object");
	

	POINT mousePos;
	GetCursorPos(&mousePos);
	XMVECTOR vec = camera->ScreenPointToRay(mousePos);


	if (ImGui::Begin("LockCamera"))
	{
		bool isLockCamera = camera->GetCameraLock();
		ImGui::Checkbox("CameraLock", &isLockCamera);
		camera->SetCameraLock(isLockCamera);
	}

	obj->Update(dt);

	uiBg->Update(dt);
	uiStart->Update(dt);
	uiOption->Update(dt);
	uiExit->Update(dt);


	ImGui::End();
}

void SceneBlank::Draw()
{
	GameApp::SetBlendState(nullptr);
	GameApp::SetCullingMode(nullptr);
	Object* obj = GetObj<Object>("Object");
	obj->Draw();
	floor->Draw();

	GameApp::SetBlendState(RenderState::BSTransparent);
	GameApp::SetCullingMode(RenderState::RSNoCull);
	primitive->Draw();

	uiBg->Draw();
	uiStart->Draw();
	uiOption->Draw();
	uiExit->Draw();
}

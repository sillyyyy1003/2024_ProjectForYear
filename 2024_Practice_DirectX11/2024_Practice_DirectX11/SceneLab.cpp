#include "SceneLab.h"
#include <memory>

#include "Circle.h"
#include "Container.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "Geometry.h"
#include "GUI.h"
#include "Object.h"
#include "Plane3D.h"
#include "RenderState.h"
#include "UI_Button.h"


using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneLab::Init()
{
	//ÉJÉÅÉâçÏê¨
	FirstPersonCamera* camera = GetObj<FirstPersonCamera>("Camera");
	camera->LookAt({ 0.0f, 5.0f, -5.0f }, { 0,0,0 }, { 0,1,0 });

	Material mat = {
		Color(0.470f, 0.31f, 0.038f, 1.0f), 	// Ambient: 
		Color(0.780f, 0.568f, 0.113f, 1.0f), 	// Diffuse: 
		Color(0.992f, 0.941f, 0.807f, 32.0f), 	// Specular: 
		Color(0.0f, 0.0f, 0.0f, 0.0f)        	// Emi:
	};

	Object* obj = CreateObj<Container>("Container");
	obj->Init(CYLINDER_ONECAP, "Assets/Texture/Lab/rust_coarse_01_diff_1k.jpg");
	obj->SetMaterial(mat);

	///êÖÇÃìßñæìxÇ‚îΩéÀÇÕëºÇ∆à·Ç§
	mat = {
   Color(0.5f, 0.5f, 0.5f, 1.0f),
   Color(1.0f, 1.0f, 1.0f, 0.5f),
   Color(0.8f, 0.8f, 0.8f, 32.0f),
   Color(0.0f, 0.0f, 0.0f, 0.0f)
	};
	water = std::make_unique<Circle>();
	water->Init("Assets/Texture/Lab/water.png");
	water->SetScale({ 10,10 });
	water->SetMaterial(mat);

	floor = std::make_unique<Plane3D>();
	floor->Init("Assets/Texture/square_tiles_02_diff_1k.jpg", Vector2(2, 2));
	floor->SetScale({ 10,10 });
	floor->mTransform.SetPosition(0, -1.0f, 0);

	table = std::make_unique<Plane3D>();
	table->Init("Assets/Texture/Lab/worn_planks_diff_1k.jpg", Vector2(1, 1));
	table->SetScale(Vector2(15, 15));
	table->SetPosition(0.f, -1.0f, 0.0f);


}

void SceneLab::UnInit()
{
	
}

void SceneLab::Update(float dt)
{
	FirstPersonCamera* camera = GetObj<FirstPersonCamera>("Camera");
	Object* obj = GetObj<Container>("Container");


	obj->Update(dt);
	camera->Update(dt);

#ifdef _DEBUG

	if (ImGui::Begin("Water Option"))
	{
		float waterPos[3] = { water->GetPosition().x,water->GetPosition().y,water->GetPosition().z };
		ImGui::InputFloat3("Position", waterPos);
		water->mTransform.SetPosition(waterPos);

		float waterScale[2] = { water->GetScale().x,water->GetScale().z };
		ImGui::InputFloat2("Scale", waterScale);
		water->mTransform.SetScaleXZ(waterScale[0], waterScale[1]);
	}
	ImGui::End();


	if (ImGui::Begin("Floor Option"))
	{
		float floorPos[3] = { floor->GetPosition().x,floor->GetPosition().y,floor->GetPosition().z };
		ImGui::InputFloat3("Position", floorPos);
		floor->mTransform.SetPosition(floorPos);

		float floorScale[2] = { floor->GetScale().x,floor->GetScale().z };
		ImGui::InputFloat2("Scale", floorScale);
		floor->mTransform.SetScaleXZ(floorScale[0], floorScale[1]);
	}
	ImGui::End();

	GUI::ObjectSetting(obj, "Container");



#endif


}

void SceneLab::Draw()
{
	// àÍî 
	GameApp::SetBlendState(nullptr);
	GameApp::SetCullingMode(nullptr);
	Object* obj = GetObj<Object>("Container");
	floor->Draw();
	table->Draw();

	// ìßñæ
	obj->Draw();
	GameApp::SetCullingMode(RenderState::RSNoCull);
	GameApp::SetBlendState(RenderState::BSTransparent);
	water->Draw();

}

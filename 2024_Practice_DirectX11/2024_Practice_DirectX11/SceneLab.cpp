#include "SceneLab.h"
#include <memory>
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "Geometry.h"
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


	Object* obj = CreateObj<Object>("Object");
	obj->Init(SPHERE, "Assets/Texture/uv-grid-tester_base1.png");

	Material mat = {
	Color(0.5f, 0.5f, 0.5f, 1.0f),
	Color(1.0f, 1.0f, 1.0f, 0.5f),
	Color(0.8f, 0.8f, 0.8f, 32.0f),
	Color(0.0f, 0.0f, 0.0f, 0.0f)
	};
	water = std::make_unique<Plane3D>();
	water->InitPlane("Assets/Texture/water.png", Vector2(5, 5));
	water->SetScale({ 10,10 });
	water->SetMaterial(mat);

	floor = std::make_unique<Plane3D>();
	floor->InitPlane("Assets/Texture/square_tiles_02_diff_1k.jpg", Vector2(2, 2));
	floor->SetScale({ 10,10 });
	floor->mTransform.SetPosition(0, -1.0f, 0);

}

void SceneLab::UnInit()
{
	
}

void SceneLab::Update(float dt)
{
	FirstPersonCamera* camera = GetObj<FirstPersonCamera>("Camera");
	Object* obj = GetObj<Object>("Object");
	

	obj->Update(dt);
	camera->Update(dt);


	if(ImGui::Begin("Water Option"))
	{
		float waterPos[3] = { water->GetPosition().x,water->GetPosition().y,water->GetPosition().z };
		ImGui::InputFloat3("Position", waterPos);
		water->mTransform.SetPosition(waterPos);

		float waterScale[2] = { water->GetScale().x,water->GetScale().z };
		ImGui::InputFloat2("Scale", waterScale);
		water->mTransform.SetScaleXZ(waterScale[0],waterScale[1]);


	}



	if (ImGui::Begin("Floor Option"))
	{
		float waterPos[3] = {floor->GetPosition().x,floor->GetPosition().y,floor->GetPosition().z };
		ImGui::InputFloat3("Position", waterPos);
		floor->mTransform.SetPosition(waterPos);

		float waterScale[2] = { floor->GetScale().x,floor->GetScale().z };
		ImGui::InputFloat2("Scale", waterScale);
		floor->mTransform.SetScaleXZ(waterScale[0], waterScale[1]);
	}



	ImGui::End();
	ImGui::End();

}

void SceneLab::Draw()
{
	GameApp::SetBlendState(nullptr);
	GameApp::SetCullingMode(nullptr);
	Object* obj = GetObj<Object>("Object");
	obj->Draw();
	floor->Draw();

	GameApp::SetBlendState(RenderState::BSTransparent);
	GameApp::SetCullingMode(RenderState::RSNoCull);
	water->Draw();

}

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

	//
	Object* pot = CreateObj<Container>("Pot");
	pot->Init(CYLINDER_ONECAP, "Assets/Texture/Lab/rust_coarse_01_diff_1k.jpg");
	pot->SetMaterial(mat);

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

	//wave = std::make_unique<Plane3D>();
	//wave->Init("Assets/Texture/Lab/water.png",20);
	//wave->SetScale({ 10,10 });
	//wave->mTransform.SetPosition(0, 1.0f, 0);
	//wave->SetMaterial(mat);

	wave = std::make_unique<Water>();
	wave->Init("Assets/Texture/Lab/water.png");
	wave->GetModel()->SetScale({ 6,6 });





	table = std::make_unique<Plane3D>();
	table->Init("Assets/Texture/Lab/worn_planks_diff_1k.jpg", Vector2(1, 1));
	table->SetScale(Vector2(15, 15));
	table->SetPosition(0.f, -1.0f, 0.0f);


}

void SceneLab::UnInit()
{
	// Create scene Data
	json sceneData;

	//Save Pot
	sceneData["Container"] = SaveData(GetObj<Container>("Pot"));
	//Save Water
	


	std::ofstream file("Assets/Data/SaveDat/scene_lab.json");
	if (file.is_open())
	{
		file << sceneData.dump(4);
		file.close();
	}
	else
	{
		DebugLog::Log("Failed to open scene_title.json for writing.");
	}
}

void SceneLab::Update(float dt)
{
	FirstPersonCamera* camera = GetObj<FirstPersonCamera>("Camera");
	Object* obj = GetObj<Container>("Pot");

	wave->Update(dt);
	table->Update(dt);
	water->Update(dt);
	//wave->Update(dt);
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

	GUI::ObjectSetting(obj, "Container");



#endif


}

void SceneLab::Draw()
{
	// àÍî 
	GameApp::SetBlendState(nullptr);
	GameApp::SetCullingMode(nullptr);
	Object* obj = GetObj<Container>("Pot");
	
	table->Draw();
	
	// ìßñæ
	obj->Draw();
	//GameApp::SetCullingMode(RenderState::RSNoCull);
	//GameApp::SetBlendState(RenderState::BSTransparent);
	//water->Draw();
	wave->Draw();

}

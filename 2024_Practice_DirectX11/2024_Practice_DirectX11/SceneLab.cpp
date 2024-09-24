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
	json sceneData=LoadSceneData("Assets/Data/SaveDat/scene_lab.json");
	//ÉJÉÅÉâçÏê¨
	FirstPersonCamera* camera = GetObj<FirstPersonCamera>("Camera");
	camera->LookAt({ 0.0f, 5.0f, -5.0f }, { 0,0,0 }, { 0,1,0 });
	camera->LockCameraAngle(true);

	Object* pot = CreateObj<Container>("Pot");
	pot->Init(CYLINDER_ONECAP);
	pot->LoadSaveData(sceneData, "Pot");

	water = std::make_unique<Water>();
	water->LoadSaveData(sceneData, "Water");


	table = std::make_unique<Plane3D>();
	table->Init("Assets/Texture/Lab/worn_planks_diff_1k.jpg", Vector2(1, 1));
	table->SetScale(Vector2(25, 15));
	table->SetPosition(0.f, -1.0f, 0.0f);


}

void SceneLab::UnInit()
{
	// Create scene Data
	json sceneData;

	//Save Pot
	sceneData["Pot"] = GetObj<Container>("Pot")->SaveData();
	//Save Water
	sceneData["Water"] = water->SaveData();

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

	water->Update(dt);
	table->Update(dt);

	obj->Update(dt);
	camera->Update(dt);

#ifdef _DEBUG



	GUI::ObjectSetting(obj, "Container");

#endif


}

void SceneLab::Draw()
{
	// àÍî 
	Object* obj = GetObj<Container>("Pot");

	GameApp::SetBlendState(nullptr);
	GameApp::SetCullingMode(nullptr);
	table->Draw();

	// ìßñæ
	GameApp::SetCullingMode(RenderState::RSNoCull);	//ó†ï\óºñ ï`âÊÇ∑ÇÈ
	GameApp::SetBlendState(RenderState::BSTransparent);	//ìßñæê›íË
	obj->Draw();
	water->Draw();

}

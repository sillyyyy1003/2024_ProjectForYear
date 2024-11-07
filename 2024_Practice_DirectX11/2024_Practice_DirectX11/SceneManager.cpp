#include "SceneManager.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "Geometry.h"
#include "Model.h"
#include "PBRModel.h"
#include "SceneLab.h"
#include "SceneTitle.h"
using namespace DirectX::SimpleMath;

//定数定義
enum SceneName
{
	SCENE_TITLE = 0,
	SCENE_LAB = 1,
	SCENE_MAX,
};


void SceneManager::Init()
{
	//LoadSaveData
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_manager.json");

	//Set SceneMap
	mSceneMap["Title"] = SCENE_TITLE;
	mSceneMap["Lab"] = SCENE_LAB;

	//カメラ作成
	std::shared_ptr<FirstPersonCamera> camera = CreateObj<FirstPersonCamera>("DefaultCamera");
	camera->LoadSaveData(sceneData, "DefaultCamera");
	camera->SetPosition(0.0, 2.0, -2.0);
	camera->LookAt(camera->GetPos(), { 0,0,0 }, camera->GetDefaultUpAxis());

	//基底ライト作成 Ambient Light
	std::shared_ptr<DirLight> light = CreateObj<DirLight>("Light");
	light->SetAmbient(Vector4(0.7f,0.7f,0.7f,1.0f));
	light->SetPos(Vector3(0.0f,10.0f,-10.0f));
	light->SetDir(Vector3(0.0f,0.0,0.f));

	//std::shared_ptr<Model> model= CreateObj<Model>("Model");
	//GetObj<Model>("Model")->Init("Assets/Model/test.obj");
	//GetObj<Model>("Model")->mTransform.SetPosition(0.0f, 0.0f, 0.0f);
	//GetObj<Model>("Model")->SetScale(100, 100, 100);


	std::shared_ptr<PBRModel> testModel = CreateObj<PBRModel>("testModel");
	GetObj<Model>("testModel")->Init("Assets/Model/LabAssets/Bucket.obj");
	GetObj<Model>("testModel")->mTransform.SetPosition(2.0f, 2.f, 0.0f);

	
	ChangeScene();
}

void SceneManager::UnInit()
{
	json sceneData;
	sceneData["DefaultCamera"] = GetObj<FirstPersonCamera>("DefaultCamera")->SaveData();

	//Save data here
	SaveSceneFile("Assets/Data/SaveDat/scene_manager.json", sceneData);

}

void SceneManager::Update(float dt)
{
	GetObj<FirstPersonCamera>("DefaultCamera")->Update(dt);
	GetObj<DirLight>("Light")->Update(dt);
}

void SceneManager::Draw()
{

}

void SceneManager::SetScene(std::string sceneName)
{
	if (mSceneMap.contains(sceneName))
	{
		mIndex = mSceneMap[sceneName];
	}

	ChangeScene();
}

void SceneManager::ChangeScene()
{
	switch (mIndex)
	{
		default:
		case SCENE_TITLE:AddSubScene<SceneTitle>();
		DebugLog::Log("SceneName = SceneTitle");
		break;
		case SCENE_LAB:AddSubScene<SceneLab>();
		DebugLog::Log("SceneName = SceneLab");
		break;
	}
}

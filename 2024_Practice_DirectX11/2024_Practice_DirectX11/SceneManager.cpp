#include "SceneManager.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "Model.h"
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
	//Set SceneMap
	mSceneMap["Title"] = SCENE_TITLE;
	mSceneMap["Lab"] = SCENE_LAB;

	//カメラ作成
	FirstPersonCamera* camera = CreateObj<FirstPersonCamera>("Camera");
	camera->SetPosition(0.0f, 5.0f, -10.0f);
	camera->LookAt({ 0.0f, 3.0f, -10.0f }, { 0,0,0 }, { 0,1,0 });

	//基底ライト作成
	DirLight* light = CreateObj<DirLight>("Light");
	light->SetAmbient(Vector4(0.7f,0.7f,0.7f,1.0f));
	light->SetPos(Vector3(5.0f,5.0f,0.0f));
	light->SetDir(Vector3(0.0f,0.0f,0.f));

	Model* model = CreateObj<Model>("Model");
	GetObj<Model>("Model")->Load("Assets/Model/Player.obj");
	GetObj<Model>("Model")->mTransform.SetPosition(1.0f, 1.0f, 0.0f);

	ChangeScene();
}

void SceneManager::UnInit()
{
	//Save data here


}

void SceneManager::Update(float dt)
{
	FirstPersonCamera* camera = GetObj<FirstPersonCamera>("Camera");
	DirLight* light = GetObj<DirLight>("Light");

	camera->Update(dt);
	light->Update(dt);
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

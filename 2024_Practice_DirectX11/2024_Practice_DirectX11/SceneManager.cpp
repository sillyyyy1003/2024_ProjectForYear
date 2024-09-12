#include "SceneManager.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "Model.h"
#include "SceneBlank.h"
using namespace DirectX::SimpleMath;

//定数定義
enum SceneName
{
	SCENE_BLANK,
	SCENE_MAX
};


void SceneManager::Init()
{
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

void SceneManager::ChangeScene()
{
	switch (mIndex)
	{
		default:
		case SCENE_BLANK:AddSubScene<SceneBlank>();
			DebugLog::Log("SceneName = SceneBlank");
		break;
	}
}

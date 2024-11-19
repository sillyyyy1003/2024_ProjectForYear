#include "SceneManager.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "Geometry.h"
#include "Model.h"
#include "PBRModel.h"
#include "SceneGame.h"
#include "SceneLab.h"
#include "SceneMission.h"
#include "SceneOption.h"
#include "ScenePotion.h"
#include "SceneTitle.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneManager::SetCurrentCamera(std::shared_ptr<CameraBase> camera)
{
	mCurrentCamera = camera;
}

void SceneManager::Init()
{
	//LoadSaveData
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_manager.json");

	//Set Scene Map
	InitSceneMap();

	//カメラ作成
	std::shared_ptr<FirstPersonCamera> camera = CreateObj<FirstPersonCamera>("DefaultCamera");
	camera->LoadSaveData(sceneData, "DefaultCamera");
	camera->SetPosition(0.0, 2.0, -2.0);
	camera->LookAt(camera->GetPos(), { 0,0,0 }, camera->GetDefaultUpAxis());
	//現在のカメラをセットする
	SetCurrentCamera(camera);

	//基底ライト作成 Ambient Light
	std::shared_ptr <DirLight> light = CreateObj<DirLight>("EnvironmentLight");
	light->SetAmbient(Vector4(0.5f,0.5f,0.5f,1.0f));
	light->SetPosition(Vector3(0.0f,10.0f,-10.0f));
	light->SetDirection(Vector3(0.0f,0.0,0.f));

	//Font Lib 作成
	InitFontLib();
	InitModelTexture();

	LoadPixelShaderFile();
	LoadVertexShaderFile();

	SetMainScene("Title");

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
	GetObj<DirLight>("EnvironmentLight")->Update(dt);

	MainSceneChangeListener();
}

void SceneManager::Draw()
{

}

void SceneManager::SetMainScene(const std::string& sceneName)
{
	if (mSceneMap.contains(sceneName))
	{
		mSceneIndex = mSceneMap[sceneName];
		ChangeMainScene();
	}
}

void SceneManager::InitSceneMap()
{	//Set SceneMap
	mSceneMap["Title"] = SceneConfig::SceneIndex::SCENE_TITLE;
	mSceneMap["Lab"] = SceneConfig::SceneIndex::SCENE_LAB;
	mSceneMap["Option"] = SceneConfig::SceneIndex::SCENE_OPTION;

	//Set SubSceneMap
	mSceneMap["Potion"] = SceneConfig::SceneIndex::SCENE_POTION;
	mSceneMap["Mission"] = SceneConfig::SceneIndex::SCENE_MISSION;
	mSceneMap["InGameOption"] = SceneConfig::SceneIndex::SCENE_IN_GAME_OPTION;

}

void SceneManager::InitFontLib()
{
	//UIFont
	UIFont_StringLiteral = CreateObj<Texture>("UIFont_StringLiteral");
	HR(UIFont_StringLiteral->Create("Assets/Texture/UI/UIFontLib/ASCIILib_StringLiteral_Regular.png"));

	UIFont_OCRA_Extend = CreateObj<Texture>("UIFont_OCRA_Extend");
	HR(UIFont_OCRA_Extend->Create("Assets/Texture/UI/UIFontLib/ASCIILib_OCRA_Extend.png"));

	UIFont_Courier_New_Regular = CreateObj<Texture>("UIFont_Courier_New_Regular");
	HR(UIFont_Courier_New_Regular->Create("Assets/Texture/UI/UIFontLib/ASCIILib_Courier_New_Regular.png"));

	//UIFont_Courier_New_Bold = CreateObj<Texture>("UIFont_Courier_New_Bold");
	//HR(UIFont_Courier_New_Bold->Create("Assets/Texture/UI/UIFontLib/ASCIILib_Courier_New_Bold.png"));

	//UIFont_Courier_New_It = CreateObj<Texture>("UIFont_Courier_New_It");
	//HR(UIFont_Courier_New_It->Create("Assets/Texture/UI/UIFontLib/ASCIILib_Courier_New_It.png"));

	//UIFont_Source_Code_Pro_It = CreateObj<Texture>("UIFont_SourceCodeProIt");
	//HR(UIFont_Source_Code_Pro_It->Create("Assets/Texture/UI/UIFontLib/ASCIILib_SourceCodeProIt.png"));


}

void SceneManager::InitModelTexture()
{
	pbrAlbedoTexture = CreateObj<Texture>("pbrAlbedo");
	HR(pbrAlbedoTexture->Create("Assets/Model/LabAssets/TrimSheets_mTrimSheet_AlbedoTransparency.png"));

	pbrMetallicTexture = CreateObj<Texture>("pbrMetallic");
	HR(pbrMetallicTexture->Create("Assets/Model/LabAssets/TrimSheets_mTrimSheet_MetallicSmoothness.png"));

	pbrNormalTexture = CreateObj<Texture>("pbrNormal");
	HR(pbrNormalTexture->Create("Assets/Model/LabAssets/TrimSheets_mTrimSheet_Normal.png"));

	pbrAOTexture = CreateObj<Texture>("pbrAO");
	HR(pbrAOTexture->Create("Assets/Model/LabAssets/mTrimSheet_Mixed_AO.png"));


	waterTexture = CreateObj<Texture>("water");
	HR(waterTexture->Create("Assets/Texture/water.png"));

	paperTexture = CreateObj<Texture>("paper");
	HR(paperTexture->Create("Assets/Texture/sepia-plasterboard-texture.jpg"));

}


void SceneManager::LoadPixelShaderFile()
{
	std::shared_ptr<PixelShader> buttonUI = CreateObj<PixelShader>("PS_ButtonUI");
	buttonUI->LoadShaderFile("Assets/Shader/PS_ButtonUI.cso");
	psList.push_back(buttonUI);

	std::shared_ptr<PixelShader> defaultUI = CreateObj<PixelShader>("PS_DefaultUI");
	defaultUI->LoadShaderFile("Assets/Shader/PS_DefaultUI.cso");
	psList.push_back(defaultUI);

	std::shared_ptr<PixelShader> object = CreateObj<PixelShader>("PS_Object");
	object->LoadShaderFile("Assets/Shader/PS_Object.cso");
	psList.push_back(object);

	std::shared_ptr<PixelShader> pbrModel = CreateObj<PixelShader>("PS_PBRModel");
	pbrModel->LoadShaderFile("Assets/Shader/PS_PBRModel.cso");
	psList.push_back(pbrModel);

	std::shared_ptr<PixelShader> primitives = CreateObj<PixelShader>("PS_Primitives");
	primitives->LoadShaderFile("Assets/Shader/PS_Primitives.cso");
	psList.push_back(primitives);

	std::shared_ptr<PixelShader> water = CreateObj<PixelShader>("Water");
	water->LoadShaderFile("Assets/Shader/PS_Water.cso");
	psList.push_back(water);


}

void SceneManager::LoadVertexShaderFile()
{
	std::shared_ptr<VertexShader> defaultUI = CreateObj<VertexShader>("VS_DefaultUI");
	defaultUI->LoadShaderFile("Assets/Shader/VS_DefaultUI.cso");
	vsList.push_back(defaultUI);

	std::shared_ptr<VertexShader> pbrModel = CreateObj<VertexShader>("VS_PBRModel");
	pbrModel->LoadShaderFile("Assets/Shader/VS_PBRModel.cso");
	vsList.push_back(pbrModel);

	std::shared_ptr<VertexShader> primitives = CreateObj<VertexShader>("VS_Primitives");
	primitives->LoadShaderFile("Assets/Shader/VS_Primitives.cso");
	vsList.push_back(primitives);

	std::shared_ptr<VertexShader> water = CreateObj<VertexShader>("VS_Water");
	water->LoadShaderFile("Assets/Shader/VS_Water.cso");
	vsList.push_back(water);
}

void SceneManager::ChangeMainScene()
{
	isChangeMainScene = true;
}

void SceneManager::ClearMainSceneChange()
{
	isChangeMainScene = false;

}

void SceneManager::MainSceneChangeListener()
{
	if (isChangeMainScene)
	{
		switch (mSceneIndex)
		{
		default:
		case SceneConfig::SceneIndex::SCENE_TITLE:AddSubScene<SceneTitle>();
			DebugLog::Log("SceneName = SceneTitle");
			break;
		case SceneConfig::SceneIndex::SCENE_LAB:AddSubScene<SceneLab>();
			DebugLog::Log("SceneName = SceneLab");
			break;
		case SceneConfig::SceneIndex::SCENE_OPTION:AddSubScene<SceneOption>();
			DebugLog::Log("SceneName = SceneOption");
			break;
		case SceneConfig::SceneIndex::SCENE_MISSION:AddSubScene<SceneMission>();
			DebugLog::Log("SceneName = SceneMission");
			break;
		case SceneConfig::SceneIndex::SCENE_POTION:AddSubScene<ScenePotion>();
			DebugLog::Log("SceneName = ScenePotion");
			break;
		case SceneConfig::SceneIndex::EXIT:
			DebugLog::Log("Exiting the application...");
			PostQuitMessage(0);
			break;
		}

		ClearMainSceneChange();
	}
}

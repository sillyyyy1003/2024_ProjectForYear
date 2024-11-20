#include "SceneLab.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "KInput.h"
#include "RenderState.h"
#include "ScenePotion.h"
#include "SceneMission.h"
#include "Sprite.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneLab::Init()
{
	//Load Save Data
	json sceneData = LoadSceneData("Assets/Data/SaveDat/SubScene_Lab.json");
	
	//Init Lab Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0, 7, -7);
	GetObj<FirstPersonCamera>("DefaultCamera")->LookAt({ 0,0,7.5 });

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData,"EnvironmentLight");

	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");
	pbrTexList[PBRConfig::PBRTex::AO] = GetObj<Texture>("pbrAO");

	//Init Model & Objects
	mPot = std::make_shared<InteractiveStaticObject>();
	mPot->Init("Assets/Model/LabAssets/Pot.obj", "pot");
	mPot->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_InterActiveObjectPBRModel.cso");
	mPot->LoadTex(pbrTexList);
	mPot->LoadSaveData(sceneData,"Pot");

	std::shared_ptr<StaticObject> Candle1 = CreateObj<StaticObject>("Candle1");
	Candle1->Init("Assets/Model/LabAssets/Candle.obj", "Candle1");
	Candle1->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Candle1->LoadTex(pbrTexList);
	Candle1->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject>	FryingStand = std::make_shared<StaticObject>();
	FryingStand->Init("Assets/Model/LabAssets/FryingStand.obj", "FryingStand");
	FryingStand->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	FryingStand->LoadTex(pbrTexList);
	FryingStand->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Candle2 = CreateObj<StaticObject>("Candle2");
	Candle2->Init("Assets/Model/LabAssets/Candle.obj", "Candle2");
	Candle2->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Candle2->LoadTex(pbrTexList);
	Candle2->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Bottle = CreateObj<StaticObject>("Bottle");
	Bottle->Init("Assets/Model/LabAssets/Bottle.obj", "Bottle");
	Bottle->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Bottle->LoadTex(pbrTexList);
	Bottle->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Jug = CreateObj<StaticObject>("Jug");
	Jug->Init("Assets/Model/LabAssets/Jug.obj", "Jug");
	Jug->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Jug->LoadTex(pbrTexList);
	Jug->LoadSaveData(sceneData);

	std::shared_ptr<StaticObject> Candle3 = CreateObj<StaticObject>("Candle3");
	Candle3->Init("Assets/Model/LabAssets/Candle.obj", "Candle3");
	Candle3->LoadShaderFile("Assets/Shader/VS_PBRModel.cso", "Assets/Shader/PS_PBRModel.cso");
	Candle3->LoadTex(pbrTexList);
	Candle3->LoadSaveData(sceneData);


	staticObjList[Candle1->GetObjectName()] = Candle1;
	staticObjList[Candle2->GetObjectName()] = Candle2;
	staticObjList[Candle3->GetObjectName()] = Candle3;
	staticObjList[FryingStand->GetObjectName()] = FryingStand;
	staticObjList[Bottle->GetObjectName()] = Bottle;
	staticObjList[Jug->GetObjectName()] = Jug;

	mTable = std::make_unique<Square>();
	mTable->Init("Assets/Texture/Lab/worn_planks_diff_1k.jpg", Vector2(1, 1));
	mTable->LoadDefShader("Assets/Shader/VS_Primitives.cso", "Assets/Shader/PS_Primitives.cso");
	mTable->SetScale(Vector2(30, 15));
	mTable->SetPosition(0.f, -0.05f, 0.0f);

	mWater = std::make_unique<Water>();
	mWater->LoadSaveData(sceneData,"water");
	mWater->SetTexture(GetObj<Texture>("water"));

	testPaper = std::make_unique<MissionPaper>();
	testPaper->Init(GetObj<Texture>("paper"),"paper");

	testPaper->SetPosition({0, 3, 7.5});
	testPaper->SetScale({2,2});

	InitTestObj();
}

void SceneLab::UnInit()
{
	json sceneData;

	for(const auto& obj:staticObjList)
	{
		sceneData[obj.first.c_str()] = obj.second->SaveData();
	}

	sceneData["Pot"] = mPot->SaveData();

	//Save EnvironmentLight
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();
	sceneData["water"] = mWater->SaveData();

	SaveSceneFile("Assets/Data/SaveDat/SubScene_Lab.json",sceneData);
}

void SceneLab::Update(float dt)
{
	TriggerListener();
	GameObjectUpdate(dt);
	mCube->Update(dt);
}

void SceneLab::Draw()
{
	/*
	//Use Transparent
	GameApp::SetCullingMode(RenderState::RSNoCull);		//裏表両面描画する
	GameApp::SetBlendState(RenderState::BSTransparent);	//透明設定
	mPot->Draw();
	mWater->Draw();
	*/
	//Normal
	GameApp::SetCullingMode(nullptr);
	mTable->Draw();
	//for (const auto& obj : staticObjList)
	//{
	//	obj.second->Draw();
	//}

	testPaper->Draw();
	
	DrawTestObj();
}

void SceneLab::GameObjectUpdate(float dt)
{
	mPot->Update(dt);

	for (const auto& obj : staticObjList)
	{
		obj.second->Update(dt);
	}

	mTable->Update(dt);

	mWater->Update(dt);
	
	testPaper->Update(dt);
}

void SceneLab::TriggerListener()
{

	if(KInput::IsKeyTrigger(VK_ESCAPE))
	{
		SceneManager::Get()->SetMainScene("Title");
		return;
	}


	if(mPot->GetClicked())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Potion");
	}

	if(testPaper->GetClicked())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Mission");
	}



}

void SceneLab::InitTestObj()
{
	mCube = std::make_unique<Cube>();
	mCube->Init(nullptr);
	mCube->LoadDefShader(GetObj<VertexShader>("VS_Primitives"),GetObj<PixelShader>("PS_Primitives"));
	mCube->SetPosition(4,0.5f,0.0f);


	mCylinder = std::make_unique<Cylinder>();
	mCylinder->Init(nullptr);
	mCylinder->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mCylinder->SetPosition(-4, 0.5f, 0.0f);


	std::shared_ptr<VertexShader> VS_SpriteShadow = CreateObj<VertexShader>("VS_SpriteShadow");
	std::shared_ptr<PixelShader> PS_WriteDepth = CreateObj<PixelShader>("PS_WriteDepth");
	std::shared_ptr<PixelShader> PS_Shadow = CreateObj<PixelShader>("PS_Shadow");

	VS_SpriteShadow->LoadShaderFile("Assets/Shader/VS_SpriteShadow.cso");
	PS_WriteDepth->LoadShaderFile("Assets/Shader/PS_WriteDepth.cso");
	PS_Shadow->LoadShaderFile("Assets/Shader/PS_Shadow.cso");


	// レンダーターゲット作成
	std::shared_ptr<RenderTarget> pRTV = CreateObj<RenderTarget>("DepthWriteRTV");
	// 奥行の情報を細かく保存するために、R8G8B8A8の
	// 各8bitではなくR32のデカい区切りで書き込みする
	pRTV->Create(DXGI_FORMAT_R32_FLOAT, gD3D->GetWinWidth(), gD3D->GetWinHeight());
	// 深度バッファ
	std::shared_ptr<DepthStencil> pDSV = CreateObj<DepthStencil>("DepthWriteDSV");
	pDSV->Create(gD3D->GetWinWidth(), gD3D->GetWinHeight(), false);

	Sprite::SetVertexShader(nullptr);
	Sprite::SetPixelShader(nullptr);
}

void SceneLab::DrawTestObj()
{
	
	std::shared_ptr<RenderTarget> pDeptWriteRTV = GetObj<RenderTarget>("DepthWriteRTV");
	std::shared_ptr<DepthStencil> pDepthWriteDSV = GetObj<DepthStencil>("DepthWriteDSV");

	XMFLOAT3 shadowPos = { mCube->GetPosition().x,0.1f,mCube->GetPosition().z };
	XMFLOAT4X4 mat[3];

	//Create Shadow 
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));

	Matrix scaleBaseMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	mat[0] = mat[0] * scaleBaseMatrix;

	XMFLOAT4X4 LMatrix[3];
	LMatrix[0] = mat[0];
	XMFLOAT3 LPos = GetObj<DirLight>("EnvironmentLight")->GetPosition();
	XMFLOAT3 LDir = GetObj<DirLight>("EnvironmentLight")->GetDirection();


	XMStoreFloat4x4(&LMatrix[1], 
		XMMatrixTranspose(
			XMMatrixLookAtLH(
				XMLoadFloat3(&LPos),
				XMLoadFloat3(&LDir),
				XMVectorSet(0.f, 1.f, 0.f, 0.f)
		)));

	XMStoreFloat4x4(&LMatrix[2], XMMatrixTranspose(XMMatrixOrthographicLH(
		20.f,20.f,0.1f,100.f
	)));

	float color[] = { 1,1,1,1 };
	pDeptWriteRTV->Clear(color);
	pDepthWriteDSV->Clear();

	GameApp::SetRenderTarget(1, &pDeptWriteRTV, pDepthWriteDSV.get());
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mCube->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mCube->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mCube->Draw();

	shadowPos = { mCylinder->GetPosition().x,0.1f,mCylinder->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mCylinder->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mCylinder->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mCylinder->Draw();

	shadowPos = { mPot->GetPosition().x,0.1f,mPot->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mPot->GetScale().x, mPot->GetScale().y, mPot->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mPot->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mPot->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mPot->Draw();


	for (const auto& obj : staticObjList)
	{

		shadowPos = { obj.second->GetPosition().x,0.1f,obj.second->GetPosition().z };
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
			XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
		scaleBaseMatrix = XMMatrixScaling(obj.second->GetScale().x, obj.second->GetScale().y, obj.second->GetScale().z);
		mat[0] = mat[0] * scaleBaseMatrix;
		LMatrix[0] = mat[0];
		GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
		obj.second->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
		obj.second->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
		obj.second->Draw();
		
	}


	GameApp::SetRenderTarget();

	mat[1] = GetObj<FirstPersonCamera>("DefaultCamera")->GetViewXMF();
	mat[2] = GetObj<FirstPersonCamera>("DefaultCamera")->GetProjXMF();

	mCube->SwitchToDefShader();
	mCube->Draw();
	mCylinder->SwitchToDefShader();
	mCylinder->Draw();

	mPot->SwitchToDefShader();
	mPot->Draw();

	for (const auto& obj : staticObjList)
	{
		obj.second->SwitchToDefShader();
		obj.second->Draw();

	}

	
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
	XMMatrixRotationX(XM_PIDIV2) * XMMatrixScaling(20.f, 20.f, 20.f) * XMMatrixTranslation(0,0.0,0)));

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);

	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_SpriteShadow")->WriteShader(2, LMatrix);

	Sprite::SetSize({ 3.f,3.f });
	Sprite::SetVertexShader(GetObj<VertexShader>("VS_SpriteShadow").get());
	Sprite::SetPixelShader(GetObj<PixelShader>("PS_Shadow").get());
	Sprite::SetTexture(pDeptWriteRTV);
	Sprite::Draw();
}


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
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_lab.json");
	
	//Init Lab Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0.f,8.5f,-10.f);
	GetObj<FirstPersonCamera>("DefaultCamera")->LookAt({ 0.f,0.f,7.5f });

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData,"EnvironmentLight");
	mCandleLight = std::make_unique<CandleLight>();
	mCandleLight->Init();
	mCandleLight->LoadSaveData(sceneData, "CandleLight");

	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");

	//Init Model & Objects
	mPot = std::make_unique<InteractiveStaticObject>();
	mPot->InitPBRModel("Assets/Model/LabAssets/Pot.obj", "pot");
	mPot->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mPot->LoadTex(pbrTexList);
	mPot->LoadSaveData(sceneData,"Pot");

	mRedBook = std::make_unique<InteractiveStaticObject>();
	mRedBook->InitPBRModel("Assets/Model/Book/book_close.obj", "redBook");
	mRedBook->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mRedBook->LoadSaveData(sceneData, "RedBook");
	mRedBook->SetModelRotation({0, 30.f, 0 });
	mRedBook->LoadAlbedoTex(GetObj<Texture>("closeBookRedAlbedo"));
	mRedBook->LoadNormalMapTex(GetObj<Texture>("closeBookNormal"));
	mRedBook->LoadMetallicMapTex(GetObj<Texture>("closeBookMetallic"));

	StaticObject* Candle1 = CreateObj<StaticObject>("Candle1").get();
	Candle1->Init("Assets/Model/LabAssets/Candle.obj", "Candle1");
	Candle1->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle1->LoadTex(pbrTexList);
	Candle1->LoadSaveData(sceneData);

	StaticObject* FryingStand = CreateObj<StaticObject>("FryingStand").get();
	FryingStand->Init("Assets/Model/LabAssets/FryingStand.obj", "FryingStand");
	FryingStand->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	FryingStand->LoadTex(pbrTexList);
	FryingStand->LoadSaveData(sceneData);

	StaticObject* Candle2 = CreateObj<StaticObject>("Candle2").get();
	Candle2->Init("Assets/Model/LabAssets/Candle.obj", "Candle2");
	Candle2->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle2->LoadTex(pbrTexList);
	Candle2->LoadSaveData(sceneData);

	StaticObject* Bottle = CreateObj<StaticObject>("Bottle").get();
	Bottle->Init("Assets/Model/LabAssets/Bottle.obj", "Bottle");
	Bottle->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Bottle->LoadTex(pbrTexList);
	Bottle->LoadSaveData(sceneData);

	StaticObject* Jug = CreateObj<StaticObject>("Jug").get();
	Jug->Init("Assets/Model/LabAssets/Jug.obj", "Jug");
	Jug->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Jug->LoadTex(pbrTexList);
	Jug->LoadSaveData(sceneData);

	StaticObject* Candle3 = CreateObj<StaticObject>("Candle3").get();
	Candle3->Init("Assets/Model/LabAssets/Candle.obj", "Candle3");
	Candle3->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle3->LoadTex(pbrTexList);
	Candle3->LoadSaveData(sceneData);

	staticObjList[Candle1->GetObjectName()] = Candle1;
	staticObjList[Candle2->GetObjectName()] = Candle2;
	staticObjList[Candle3->GetObjectName()] = Candle3;
	staticObjList[FryingStand->GetObjectName()] = FryingStand;
	staticObjList[Bottle->GetObjectName()] = Bottle;
	staticObjList[Jug->GetObjectName()] = Jug;

	mTable = std::make_unique<Square>();
	mTable->Init(GetObj<Texture>("table"), 0);
	mTable->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mTable->SetScale(Vector2(30.f, 15.f));
	mTable->SetPosition(0.f, -0.1f, 0.0f);

	mWater = std::make_unique<Water>();
	mWater->LoadSaveData(sceneData,"water");
	mWater->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Water"));
	mWater->SetTexture(GetObj<Texture>("water"));
	mWater->SetWaterState(WaterStateConfig::WaterState::STATE_BOILING);
	mWater->SetWaterBoilingState(WaterStateConfig::WaterBoilingState::STATE_BOILING);
	
	mMissionPaper = std::make_unique<MissionPaper>();
	mMissionPaper->Init(GetObj<Texture>("paper"),"paper");
	mMissionPaper->LoadDefShader(GetObj<VertexShader>("VS_Primitives"),GetObj<PixelShader>("PS_InteractiveObjectNormal"));
	mMissionPaper->SetPosition({0.f, 3.f, 12.f});
	mMissionPaper->SetScale({2,2});

	//Init Point Light
	
	mLightBulb2 = std::make_unique<Sphere>();
	mLightBulb2->Init(nullptr, 32, 32);
	mLightBulb2->LoadDefShader();
	mLightBulb2->SetScale(0.5f, 0.5, 0.5f);
	mLightBulb2->SetPosition(7.f, 1.5f, 3.f);

	InitShadowRenderTarget();
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
	//todo:Environment Config should be saved as a set
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();

	sceneData["water"] = mWater->SaveData();
	sceneData["RedBook"] = mRedBook->SaveData();

	sceneData["CandleLight"] = mCandleLight->SaveData();
	SaveSceneFile("Assets/Data/SaveDat/scene_lab.json",sceneData);
}

void SceneLab::Update(float dt)
{
	TriggerListener();
	GameObjectUpdate(dt);
	
}

void SceneLab::Draw()
{
	mCandleLight->Draw();
	DrawObjectsWithShadow();
}

void SceneLab::GameObjectUpdate(float dt)
{
	mWater->Update(dt);
	mMissionPaper->Update(dt);

	mPot->Update(dt);
	mRedBook->Update(dt);
	mCandleLight->Update(dt);

	for(const auto& staticObj:staticObjList)
	{
		staticObj.second->Update(dt);
	}

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

	if(mMissionPaper->GetClicked())
	{
		SceneManager::Get()->SetSwitchScene(true);
		SceneManager::Get()->SetMainScene("Mission");
	}

}

void SceneLab::InitShadowRenderTarget()
{
	mCube = std::make_unique<Cube>();
	mCube->Init(nullptr);
	mCube->LoadDefShader(GetObj<VertexShader>("VS_Primitives"),GetObj<PixelShader>("PS_Primitives"));
	mCube->SetPosition(2.5f,0.15f,-1.f);
	mCube->SetScale(1.0f, .3f, 1.f);
	mCube->SetDiffuse({ 0.5f,0.2f,0.1f,1.0f });

	mCylinder = std::make_unique<Cylinder>();
	mCylinder->Init(nullptr);
	mCylinder->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mCylinder->SetPosition(3.3f,0.75f, 0.6f);
	mCylinder->SetScale(1.f, 1.5f, 1.f);
	mCylinder->SetDiffuse({ 0.4f,0.2f,0.7f,1.0f });

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

void SceneLab::DrawObjectsWithShadow()
{
	
	std::shared_ptr<RenderTarget> pDeptWriteRTV = GetObj<RenderTarget>("DepthWriteRTV");
	DepthStencil* pDepthWriteDSV = GetObj<DepthStencil>("DepthWriteDSV").get();
	float color[] = { 1,1,1,1 };
	pDeptWriteRTV->Clear(color);
	pDepthWriteDSV->Clear();
	GameApp::SetRenderTarget(1, &pDeptWriteRTV, pDepthWriteDSV);

	XMFLOAT3 shadowPos = { 0,0,0 };
	XMFLOAT4X4 mat[3];
	XMFLOAT4X4 LMatrix[3];
	//Create Shadow 
	Matrix scaleBaseMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	// Create Light View Matrix
	XMFLOAT3 LPos = mCandleLight->GetCastShadowLightPos();
	XMFLOAT3 LDir = { 0,0,0 };
	XMStoreFloat4x4(&LMatrix[1],
		XMMatrixTranspose(
			XMMatrixLookAtLH(
				XMLoadFloat3(&LPos),
				XMLoadFloat3(&LDir),
				XMVectorSet(0.f, 1.f, 0.f, 0.f)
			)));
	// Create Light Projection Matrix
	XMStoreFloat4x4(&LMatrix[2], XMMatrixTranspose(XMMatrixOrthographicLH(
		20.f, 20.f, 0.1f, 100.f
	)));

	//Set WVP Matrix
	shadowPos = { mCube->GetPosition().x,0.0f,mCube->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mCube->GetScale().x, mCube->GetScale().y, mCube->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mCube->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mCube->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mCube->Draw();

	shadowPos = { mCylinder->GetPosition().x,0.0f,mCylinder->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mCylinder->GetScale().x, mCylinder->GetScale().y, mCylinder->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mCylinder->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mCylinder->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mCylinder->Draw();

	/*	DirectX::XMVECTOR scaleVec = XMLoadFloat3(&mScale);
	DirectX::XMVECTOR quaternion = XMLoadFloat4(&mRotation);
	DirectX::XMVECTOR positionVec = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX World = XMMatrixAffineTransformation(scaleVec, g_XMZero, quaternion, positionVec);
	return World;*/
	
	shadowPos = { mPot->GetPosition().x,0.0f,mPot->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mPot->GetScale().x, mPot->GetScale().y, mPot->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mPot->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mPot->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mPot->Draw();

	
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(mRedBook->GetTransform().GetLocalToWorldMatrixXM()));
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mRedBook->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mRedBook->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mRedBook->Draw();

	for (const auto& obj : staticObjList)
	{
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(obj.second->GetTransform().GetLocalToWorldMatrixXM()));
		LMatrix[0] = mat[0];
		GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
		obj.second->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
		obj.second->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
		obj.second->Draw();
	}
	
	GameApp::SetDefaultRenderTarget();

	//Insert PointLight;
	Light::PointLight pl[2] = {
		mCandleLight->GetPointLight(),
		{}
	};

	

	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_Primitives")->WriteShader(1, pl);
	mCube->SwitchToDefShader();
	mCube->Draw();
	mCylinder->SwitchToDefShader();
	mCylinder->Draw();
	mTable->Draw();
	mMissionPaper->Draw();
	mLightBulb2->Draw();


	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_InterActiveObjectPBRModel")->WriteShader(1, pl);
	mRedBook->SwitchToDefShader();
	mRedBook->Draw();
	mPot->SwitchToDefShader();
	mPot->Draw();
	mWater->Draw();

	//Set PointLight to pbr shader
	for (const auto& obj : staticObjList)
	{
		obj.second->SwitchToDefShader();
		obj.second->GetDefPS()->WriteShader(1, pl);
		obj.second->Draw();

	}
	
	
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixRotationX(XM_PIDIV2) * XMMatrixScaling(20.f, 20.f, 20.f) * XMMatrixTranslation(0, 0.0, 0)));
	mat[1] = GameApp::GetCurrentCamera()->GetViewXMF();
	mat[2] = GameApp::GetCurrentCamera()->GetProjXMF();
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


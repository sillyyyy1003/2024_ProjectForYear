#include "SceneLab.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "IngredientManager.h"
#include "KInput.h"
#include "MissionManager.h"
#include "RenderState.h"
#include "ScenePotion.h"
#include "SceneMission.h"
#include "ScreenFadeEffect.h"
#include "Sprite.h"
#include "TutorialManager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

enum SceneState
{
	STATE_NONE,
	STATE_POTION,
	STATE_Title,
	STATE_MISSION
};

void SceneLab::Init()
{
	//Load Save Data
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_lab.json");
	
	//Init Lab Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0.f,8.5f,-10.f);
	GetObj<FirstPersonCamera>("DefaultCamera")->LookAt({ 0.f,0.f,7.5f });
#ifdef NDEBUG
	GetObj<FirstPersonCamera>("DefaultCamera")->LockCamera();
#endif

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData,"EnvironmentLight");
	mCandleLight1 = std::make_unique<CandleLight>();
	mCandleLight1->Init();
	mCandleLight1->LoadSaveData(sceneData, "CandleLight1");
	mCandleLight1->InitName("CandleLight1");

	mCandleLight2 = std::make_unique<CandleLight>();
	mCandleLight2->Init();
	mCandleLight2->InitName("CandleLight2");
	mCandleLight2->LoadSaveData(sceneData, "CandleLight2");


	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");

	//Init Model & Objects
	mPot = std::make_unique<InteractiveStaticObject>();
	mPot->InitPBRModel("Assets/Model/LabAssets/Pot.obj", "pot");
	mPot->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mPot->LoadTex(pbrTexList);
	mPot->LoadSaveData(sceneData,"Pot");

	mPotTop = std::make_unique<StaticObject>();
	mPotTop->InitPBR("Assets/Model/LabAssets/PotTop.obj", "PotTop");
	mPotTop->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	mPotTop->LoadTex(pbrTexList);
	mPotTop->LoadSaveData(sceneData);

	StaticObject* Candle1 = CreateObj<StaticObject>("Candle1").get();
	Candle1->InitPBR("Assets/Model/LabAssets/Candle.obj", "Candle1");
	Candle1->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle1->LoadTex(pbrTexList);
	Candle1->LoadSaveData(sceneData);

	StaticObject* FryingStand = CreateObj<StaticObject>("FryingStand").get();
	FryingStand->InitPBR("Assets/Model/LabAssets/FryingStand.obj", "FryingStand");
	FryingStand->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	FryingStand->LoadTex(pbrTexList);
	FryingStand->LoadSaveData(sceneData);

	StaticObject* Candle2 = CreateObj<StaticObject>("Candle2").get();
	Candle2->InitPBR("Assets/Model/LabAssets/Candle.obj", "Candle2");
	Candle2->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle2->LoadTex(pbrTexList);
	Candle2->LoadSaveData(sceneData);

	StaticObject* Candle3 = CreateObj<StaticObject>("Candle3").get();
	Candle3->InitPBR("Assets/Model/LabAssets/Candle.obj", "Candle3");
	Candle3->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle3->LoadTex(pbrTexList);
	Candle3->LoadSaveData(sceneData);

	StaticObject* Candle4 = CreateObj<StaticObject>("Candle4").get();
	Candle4->InitPBR("Assets/Model/LabAssets/Candle.obj", "Candle4");
	Candle4->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle4->LoadTex(pbrTexList);
	Candle4->LoadSaveData(sceneData);

	StaticObject* Candle5 = CreateObj<StaticObject>("Candle5").get();
	Candle5->InitPBR("Assets/Model/LabAssets/Candle.obj", "Candle5");
	Candle5->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle5->LoadTex(pbrTexList);
	Candle5->LoadSaveData(sceneData);

	StaticObject* Candle6 = CreateObj<StaticObject>("Candle6").get();
	Candle6->InitPBR("Assets/Model/LabAssets/Candle.obj", "Candle6");
	Candle6->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Candle6->LoadTex(pbrTexList);
	Candle6->LoadSaveData(sceneData);

	StaticObject* Bottle = CreateObj<StaticObject>("Bottle").get();
	Bottle->InitPBR("Assets/Model/LabAssets/Bottle.obj", "Bottle");
	Bottle->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Bottle->LoadTex(pbrTexList);
	Bottle->LoadSaveData(sceneData);

	StaticObject* Jug = CreateObj<StaticObject>("Jug").get();
	Jug->InitPBR("Assets/Model/LabAssets/Jug.obj", "Jug");
	Jug->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_PBRModel"));
	Jug->LoadTex(pbrTexList);
	Jug->LoadSaveData(sceneData);

	rightStaticObjList[Candle1->GetObjectName()] = Candle1;
	rightStaticObjList[Candle2->GetObjectName()] = Candle2;
	rightStaticObjList[Candle3->GetObjectName()] = Candle3;
	rightStaticObjList[FryingStand->GetObjectName()] = FryingStand;
	rightStaticObjList[Jug->GetObjectName()] = Jug;

	leftStaticObjList[Bottle->GetObjectName()] = Bottle;
	leftStaticObjList[Candle4->GetObjectName()] = Candle4;
	leftStaticObjList[Candle5->GetObjectName()] = Candle5;
	leftStaticObjList[Candle6->GetObjectName()] = Candle6;

	mRedPotion = std::make_unique<Ingredient>();
	mRedPotion->InitModel("Assets/Model/Potion4.obj", "RedPotion", PrimitiveConfig::MULTI, { 1,1 });
	mRedPotion->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mRedPotion->LoadSaveData(sceneData);
	mRedPotion->SetPigmentColor(HSVtoRGB({ 0,1,0.5f }));
	mRedPotion->SetMovable(false);

	mBluePotion = std::make_unique<Ingredient>();
	mBluePotion->InitModel("Assets/Model/Potion2.obj", "BluePotion", PrimitiveConfig::MULTI, { 1,1 });
	mBluePotion->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mBluePotion->LoadSaveData(sceneData);
	mBluePotion->SetPigmentColor(HSVtoRGB({ 240,1,0.5f }));
	mBluePotion->SetMovable(false);

	mYellowPotion = std::make_unique<Ingredient>();
	mYellowPotion->InitModel("Assets/Model/Potion3.obj", "YellowPotion", PrimitiveConfig::MULTI, { 1,1 });
	mYellowPotion->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mYellowPotion->LoadSaveData(sceneData);
	mYellowPotion->SetPigmentColor(HSVtoRGB({ 60,1,0.5f }));
	mYellowPotion->SetMovable(false);

	//Potionのキャパシティを設定する
	Player* player = SceneManager::Get()->GetObj<Player>("player").get();
	mRedPotion->SetCapacity(GetObj<Player>("player")->GetCapacities(PlayerConfig::RED));
	mBluePotion->SetCapacity(GetObj<Player>("player")->GetCapacities(PlayerConfig::BLUE));
	mYellowPotion->SetCapacity(GetObj<Player>("player")->GetCapacities(PlayerConfig::YELLOW));

	mTable = std::make_unique<Square>();
	mTable->Init(GetObj<Texture>("table"), 0);
	mTable->LoadDefShader();
	mTable->SetScale(Vector2(30.f, 15.f));
	mTable->SetPosition(0.f, -0.01f, 0.0f);

	mWall = std::make_unique<StaticObject>();
	mWall->InitModel("Assets/Texture/brown-cement-concrete_base_1k.jpg", "Wall", PrimitiveConfig::SQUARE);
	mWall->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mWall->LoadSaveData(sceneData);

	mWater = std::make_unique<Water>();
	mWater->LoadSaveData(sceneData,"water");
	mWater->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Water"));
	mWater->SetTexture(GetObj<Texture>("water"));
	mWater->SetWaterState(WaterStateConfig::WaterState::STATE_BOILING);
	mWater->SetWaterBoilingState(WaterStateConfig::WaterBoilingState::STATE_BOILING);
	
	mPaperOnTable = std::make_unique<InteractiveStaticObject>();
	mPaperOnTable->Init(PrimitiveConfig::SQUARE, GetObj<Texture>("paper4"), "PaperOnTable");
	mPaperOnTable->LoadSaveData(sceneData);
	mPaperOnTable->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_InteractiveObjectNormal"));
	mPaperOnTable->SetRimLightColor({ 0.2f,0.2f,0.2f,0.2f });

	mSplash = std::make_unique<StaticObject>();
	mSplash->InitModel(GetObj<Texture>("splash"), "SplashOnTable", PrimitiveConfig::SQUARE);
	mSplash->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_InteractiveObjectNormal"));
	mSplash->LoadSaveData(sceneData);


	mText = std::make_unique<D2D_UIStackContainer>();
	mText->Init(D2DUIConfig::UIShape::RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "TextOnPaper");
	mText->SetUIState(D2DUIConfig::STATE_USE_FONT);

	//UI
	mGoldBar = std::make_unique<D2D_UIStackContainer>();
	mGoldBar->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "GoldBar");
	mGoldBar->LoadSaveData(sceneData["GoldBar"]);
	mGoldBar->SetUIState(D2DUIConfig::STATE_USE_FONT);


	//Init Shadow RenderTarget
	InitShadowRenderTarget();
}

void SceneLab::UnInit()
{
#ifdef _DEBUG
	json sceneData;

	for(const auto& obj:leftStaticObjList)
	{
		sceneData[obj.first.c_str()] = obj.second->SaveData();
	}

	for (const auto& obj : rightStaticObjList)
	{
		sceneData[obj.first.c_str()] = obj.second->SaveData();
	}

	sceneData["Pot"] = mPot->SaveData();

	//Save EnvironmentLight
	//todo:Environment Config should be saved as a set
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();

	//Save Camera Param
	sceneData["DefaultCamera"] = GetObj<FirstPersonCamera>("DefaultCamera")->SaveData();

	//Save Other Data
	sceneData["water"] = mWater->SaveData();
	sceneData["CandleLight1"] = mCandleLight1->SaveData();
	sceneData["CandleLight2"] = mCandleLight2->SaveData();
	sceneData["Wall"] = mWall->SaveData();
	sceneData["PaperOnTable"] = mPaperOnTable->SaveData();

	sceneData["TextOnTable"] = mText->SaveData();
	sceneData["SplashOnTable"] = mSplash->SaveData();

	sceneData["RedPotion"] = mRedPotion->SaveData();
	sceneData["BluePotion"] = mBluePotion->SaveData();
	sceneData["YellowPotion"] = mYellowPotion->SaveData();

	sceneData["GoldBar"] = mGoldBar->SaveData();

	sceneData["PotTop"] = mPotTop->SaveData();

	SaveSceneFile("Assets/Data/SaveDat/scene_lab.json",sceneData);
#endif
}

void SceneLab::Update(float dt)
{
	TutorialManager::Get()->Update(dt);

	//シーン静止の場合
	if (TutorialManager::Get()->GetSceneFreeze())return;

	//GameObject Update
	GameObjectUpdate(dt);

	//フェード中にシーン操作を禁止
	if(!ScreenFadeEffect::Get()->GetFade())
	{
		TriggerListener();
	}

	if(ScreenFadeEffect::Get()->GetFadeIn())
	{
		switch (mNextScene)
		{
		case SceneConfig::SceneIndex::SCENE_TITLE:
			SceneManager::Get()->SetMainScene("Title");
			break;
		case SceneConfig::SceneIndex::SCENE_MISSION:
			SceneManager::Get()->SetMainScene("Mission");
			break;
		case SceneConfig::SceneIndex::SCENE_POTION:
			SceneManager::Get()->SetMainScene("Potion");
			break;
		default:
			return;
		}
	}


}

void SceneLab::Draw()
{
	mCandleLight1->Draw();
	mCandleLight2->Draw();
	DrawObjectsWithShadow();

	mGoldBar->Draw();
	
	if(isCheckMission)mText->Draw();

	TutorialManager::Get()->Draw();

}

void SceneLab::GameObjectUpdate(float dt)
{
	//Light
	mCandleLight1->Update(dt);
	mCandleLight2->Update(dt);

	//Water
	mWater->Update(dt);

	//Interactive object
	mPot->Update(dt);
	mPotTop->Update(dt);

	//StaticObject
	mWall->Update(dt);

	for(const auto& staticObj:leftStaticObjList)
	{
		staticObj.second->Update(dt);
	}

	for (const auto& staticObj : rightStaticObjList)
	{
		staticObj.second->Update(dt);
	}

	//Potion
	mRedPotion->Update(dt);
	mYellowPotion->Update(dt);
	mBluePotion->Update(dt);

	//Mission 
	MissionManager::Get()->UpdateCurrentMissionSet(dt);

	//Ui
	mGoldBar->SetText(GetObj<Player>("player")->GetPlayerGold().c_str());
	mGoldBar->Update(dt);

	//紙の部分
	mPaperOnTable->Update(dt);
	mText->Update(dt);
	mSplash->SetDiffuseColor(MissionManager::Get()->GetCurrentMission().MissionColor);
	mSplash->Update(dt);

	
	
}

void SceneLab::TriggerListener()
{
	//Move to title Scene
	if(KInput::IsKeyTrigger(VK_ESCAPE))
	{
		if(isCheckMission)
		{
			GetObj<FirstPersonCamera>("DefaultCamera")->BackToDefaultPos();
			isCheckMission = !isCheckMission;
		}else
		{
			SceneManager::Get()->SetSwitchSceneWithFade(true);
			mNextScene = SceneConfig::SceneIndex::SCENE_TITLE;
			return;
		}
	}

	//Move to Potion Scene
	if(mPot->GetClicked())
	{
		SceneManager::Get()->SetSwitchSceneWithFade(true);
		mNextScene = SceneConfig::SceneIndex::SCENE_POTION;
		return;
	}

	//Move to Mission Scene
	if(MissionManager::Get()->MissionPaperGetClicked())
	{
		GetObj<FirstPersonCamera>("DefaultCamera")->StartMoveToTarget(mPaperOnTable->GetPosition() + DirectX::XMFLOAT3(0, 4., -1.6f), { 1.2f,0.f,0.f }, 1.f);
		isCheckMission = true;
		
	}


}


void SceneLab::InitShadowRenderTarget()
{
	//3つのレンダーターゲットに分けて描画する
	// レンダーターゲット作成/左の分
	RenderTarget* pRTVLeft = CreateObj<RenderTarget>("LeftDepthWriteRTV").get();
	// 奥行の情報を細かく保存するために、R8G8B8A8の
	// 各8bitではなくR32のデカい区切りで書き込みする
	pRTVLeft->Create(DXGI_FORMAT_R32_FLOAT, (UINT)WIN_WIDTH, (UINT)WIN_HEIGHT);
	// 深度バッファ
	DepthStencil* pLeftDSV = CreateObj<DepthStencil>("LeftDepthWriteDSV").get();
	pLeftDSV->Create((UINT)WIN_WIDTH, (UINT)WIN_HEIGHT, false);

	RenderTarget* pRTVRight = CreateObj<RenderTarget>("RightDepthWriteRTV").get();
	// 奥行の情報を細かく保存するために、R8G8B8A8の
	// 各8bitではなくR32のデカい区切りで書き込みする
	pRTVRight->Create(DXGI_FORMAT_R32_FLOAT, (UINT)WIN_WIDTH, (UINT)WIN_HEIGHT);
	// 深度バッファ
	DepthStencil* pRightDSV = CreateObj<DepthStencil>("RightDepthWriteDSV").get();
	pRightDSV->Create((UINT)WIN_WIDTH, (UINT)WIN_HEIGHT, false);

	RenderTarget* pRTVMiddle = CreateObj<RenderTarget>("MiddleDepthWriteRTV").get();
	// 奥行の情報を細かく保存するために、R8G8B8A8の
	// 各8bitではなくR32のデカい区切りで書き込みする
	pRTVMiddle->Create(DXGI_FORMAT_R32_FLOAT, (UINT)WIN_WIDTH, (UINT)WIN_HEIGHT);
	// 深度バッファ
	DepthStencil* pDSVMiddle = CreateObj<DepthStencil>("MiddleDepthWriteDSV").get();
	pDSVMiddle->Create((UINT)WIN_WIDTH, (UINT)WIN_HEIGHT, false);


	Sprite::SetVertexShader(nullptr);
	Sprite::SetPixelShader(nullptr);
}

void SceneLab::DrawObjectsWithShadow()
{	
	DrawLeftObjectWithShadow();
	DrawRightObjectWithShadow();
	DrawMiddleObjectWithShadow();
}

void SceneLab::DrawLeftObjectWithShadow()
{
	std::shared_ptr<RenderTarget> pDeptWriteRTV = GetObj<RenderTarget>("LeftDepthWriteRTV");
	DepthStencil* pDepthWriteDSV = GetObj<DepthStencil>("LeftDepthWriteDSV").get();
	float color[] = { 1,1,1,1 };
	pDeptWriteRTV->Clear(color);
	pDepthWriteDSV->Clear();
	GameApp::SetRenderTarget(1, &pDeptWriteRTV, pDepthWriteDSV);

	XMFLOAT3 shadowPos = { 0,0,0 };
	XMFLOAT4X4 mat[3];
	XMFLOAT4X4 LMatrix[3];
	// Create Light View Matrix
	XMFLOAT3 LPos = mCandleLight2->GetCastShadowLightPos();
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

	//Bottle Shadow
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(leftStaticObjList["Bottle"]->GetTransform().GetLocalToWorldMatrixXM()));
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	leftStaticObjList["Bottle"]->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	leftStaticObjList["Bottle"]->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	leftStaticObjList["Bottle"]->Draw();


	

	GameApp::SetDefaultRenderTarget();
	//Insert PointLight;
	Light::PointLight pl[2] = {
		mCandleLight1->GetPointLight(),
		mCandleLight2->GetPointLight()
	};

	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_Primitives")->WriteShader(1, pl);
	mTable->Draw();
	mWall->Draw();


	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_InterActiveObjectPBRModel")->WriteShader(1, pl);
	GetObj<PixelShader>("PS_InteractiveObjectNormal")->WriteShader(1, pl);
	MissionManager::Get()->DrawCurrentMissionSet();
	mPaperOnTable->SwitchToDefShader();
	mPaperOnTable->Draw();
	mSplash->Draw();

	//Set PointLight to pbr shader
	for (const auto& obj : leftStaticObjList)
	{
		obj.second->SwitchToDefShader();
		obj.second->GetDefPS()->WriteShader(1, pl);
		obj.second->Draw();
	}


	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixRotationX(XM_PIDIV2) * XMMatrixScaling(40.f, 40.f, 40.f) * XMMatrixTranslation(0, 0.0f, 0)));
	mat[1] = GameApp::GetCurrentCamera()->GetViewXMF();
	mat[2] = GameApp::GetCurrentCamera()->GetProjXMF();
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);

	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_SpriteShadow")->WriteShader(2, LMatrix);

	Sprite::SetSize({ 4.f,4.f });
	Sprite::SetVertexShader(GetObj<VertexShader>("VS_SpriteShadow").get());
	Sprite::SetPixelShader(GetObj<PixelShader>("PS_Shadow").get());
	Sprite::SetTexture(pDeptWriteRTV);
	GameApp::SetBlendState(RenderState::BSTransparent);
	Sprite::Draw();
}

void SceneLab::DrawRightObjectWithShadow()
{
	std::shared_ptr<RenderTarget> pRTV = GetObj<RenderTarget>("RightDepthWriteRTV");
	DepthStencil* pDepthWriteDSV = GetObj<DepthStencil>("RightDepthWriteDSV").get();
	float color[] = { 1,1,1,1 };
	pRTV->Clear(color);
	pDepthWriteDSV->Clear();
	GameApp::SetRenderTarget(1, &pRTV, pDepthWriteDSV);

	XMFLOAT3 shadowPos = { 0,0,0 };
	XMFLOAT4X4 mat[3];
	XMFLOAT4X4 LMatrix[3];
	//Create Shadow 
	Matrix scaleBaseMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	// Create Light View Matrix
	XMFLOAT3 LPos = mCandleLight1->GetCastShadowLightPos();
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

	
	//DrawJug Shadow
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(rightStaticObjList["Jug"]->GetTransform().GetLocalToWorldMatrixXM()));
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	rightStaticObjList["Jug"]->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	rightStaticObjList["Jug"]->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	rightStaticObjList["Jug"]->Draw();

	shadowPos = { mRedPotion->GetPosition().x,0.0f, mRedPotion->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mRedPotion->GetScale().x, mRedPotion->GetScale().y, mRedPotion->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mRedPotion->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mRedPotion->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mRedPotion->Draw();

	shadowPos = { mBluePotion->GetPosition().x,0.0f, mBluePotion->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mBluePotion->GetScale().x, mBluePotion->GetScale().y, mBluePotion->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mBluePotion->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mBluePotion->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mBluePotion->Draw();

	shadowPos = { mYellowPotion->GetPosition().x,0.0f, mYellowPotion->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mYellowPotion->GetScale().x, mYellowPotion->GetScale().y, mYellowPotion->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mYellowPotion->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mYellowPotion->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mYellowPotion->Draw();

	GameApp::SetDefaultRenderTarget();

	//Insert PointLight;
	Light::PointLight pl[2] = {
		mCandleLight1->GetPointLight(),
		mCandleLight2->GetPointLight()
	};

	//Set PointLight to pbr shader
	for (const auto& obj : rightStaticObjList)
	{
		obj.second->SwitchToDefShader();
		obj.second->GetDefPS()->WriteShader(1, pl);
		obj.second->Draw();
	}

	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_Ingredient")->WriteShader(1, pl);
	mRedPotion->SwitchToDefShader();
	mRedPotion->Draw();
	mBluePotion->SwitchToDefShader();
	mBluePotion->Draw();
	mYellowPotion->SwitchToDefShader();
	mYellowPotion->Draw();
	

	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixRotationX(XM_PIDIV2) * XMMatrixScaling(40.f, 40.f, 40.f) * XMMatrixTranslation(0, 0.0f, 0)));
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
	Sprite::SetTexture(pRTV);
	GameApp::SetBlendState(RenderState::BSTransparent);
	Sprite::Draw();
}

void SceneLab::DrawMiddleObjectWithShadow()
{
	std::shared_ptr<RenderTarget> pRTV = GetObj<RenderTarget>("MiddleDepthWriteRTV");
	DepthStencil* pDepthWriteDSV = GetObj<DepthStencil>("MiddleDepthWriteDSV").get();
	float color[] = { 1,1,1,1 };
	pRTV->Clear(color);
	pDepthWriteDSV->Clear();
	GameApp::SetRenderTarget(1, &pRTV, pDepthWriteDSV);

	XMFLOAT3 shadowPos = { 0,0,0 };
	XMFLOAT4X4 mat[3];
	XMFLOAT4X4 LMatrix[3];
	//Create Shadow 
	Matrix scaleBaseMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	// Create Light View Matrix
	XMFLOAT3 LPos = {0.1f,10.f,0.f};
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
	shadowPos = { mPot->GetPosition().x,0.0f,mPot->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mPot->GetScale().x * 0.8f, mPot->GetScale().y * 0.8f, mPot->GetScale().z * 0.8f);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mPot->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mPot->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mPot->Draw();

	shadowPos = { mPotTop->GetPosition().x,0.1f,mPotTop->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mPotTop->GetScale().x * 0.4f, mPotTop->GetScale().y * 0.8f, mPotTop->GetScale().z * 0.8f);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mPotTop->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mPotTop->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mPotTop->Draw();


	GameApp::SetDefaultRenderTarget();

	//Insert PointLight;
	Light::PointLight pl[2] = {
		mCandleLight1->GetPointLight(),
		mCandleLight2->GetPointLight()
	};

		
	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_InterActiveObjectPBRModel")->WriteShader(1, pl);
	GetObj<PixelShader>("PS_InteractiveObjectNormal")->WriteShader(1, pl);
	MissionManager::Get()->DrawCurrentMissionSet();
	GameApp::SetBlendState(RenderState::BSTransparent);
	mPot->SwitchToDefShader();
	mPot->Draw();
	mPotTop->SwitchToDefShader();
	mPotTop->Draw();
	mWater->Draw();


	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixRotationX(XM_PIDIV2) * XMMatrixScaling(40.f, 40.f, 40.f) * XMMatrixTranslation(0, 0.0f, 0)));
	mat[1] = GameApp::GetCurrentCamera()->GetViewXMF();
	mat[2] = GameApp::GetCurrentCamera()->GetProjXMF();
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);

	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_SpriteShadow")->WriteShader(2, LMatrix);


	Sprite::SetSize({ 4.f,4.f });
	Sprite::SetVertexShader(GetObj<VertexShader>("VS_SpriteShadow").get());
	Sprite::SetPixelShader(GetObj<PixelShader>("PS_Shadow").get());
	Sprite::SetTexture(pRTV);
	GameApp::SetBlendState(RenderState::BSTransparent);
	Sprite::Draw();
}


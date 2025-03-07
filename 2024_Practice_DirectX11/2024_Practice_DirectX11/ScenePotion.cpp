﻿#include "ScenePotion.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "IngredientManager.h"
#include "InteractiveStaticObject.h"
#include "KInput.h"
#include "Potion.h"
#include "RenderState.h"
#include "ResultManager.h"
#include "SceneManager.h"
#include "SceneScore.h"
#include "ScreenFadeEffect.h"
#include "Sprite.h"

using namespace DirectX;

void ScenePotion::Init()
{
	//Load Save Data
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_potion.json");
	json uiData = LoadSceneData("Assets/Data/SaveDat/ui_config.json");
	//Init Potion Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0, 11.12f, -4.7f);
	GetObj<FirstPersonCamera>("DefaultCamera")->mTransform.SetRotationInDegree(66.6f, 0.f, 0.f);
#ifdef NDEBUG
	GetObj<FirstPersonCamera>("DefaultCamera")->LockCamera();
#endif

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData, "EnvironmentLight");

	//==================================================//
	//===================Init object====================//
	//==================================================//
	mWater = CreateObj<Potion>("PotionSceneWater");
	mWater->SetParticleNum(100);
	mWater->LoadSaveData(sceneData, "ScenePotionWater");
	mWater->LoadShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mWater->SetTexture(GetObj<Texture>("whiteTex"));
	mWater->SetWaterBoilingState(WaterStateConfig::WaterBoilingState::STATE_CONSTANT_BOILING);
	mWater->SetWaterState(WaterStateConfig::WaterState::STATE_RIPPLING);
	mWater->InitPotionParticleEffect({ 0,0.9f,0 }, { 0,-0.3f,0 }, 3.f, 70, 0.025f);
	mWater->SetCamera(GetObj<FirstPersonCamera>("DefaultCamera").get());

	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");

	//Init Model & Objects
	mPot = CreateObj<InteractiveStaticObject>("pot");
	mPot->InitPBRModel("Assets/Model/LabAssets/Pot.obj", "pot");
	mPot->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mPot->LoadTex(pbrTexList);
	mPot->LoadSaveData(sceneData, "Pot");
	mPot->DisableRimLightEffect();

	mPotTop = std::make_unique<NonDiegeticUI_PotTop>();
	mPotTop->InitPBRModel("Assets/Model/LabAssets/PotTop.obj", "PotTop");
	mPotTop->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mPotTop->LoadTex(pbrTexList);
	mPotTop->LoadSaveData(sceneData);
	

	mTable = std::make_unique<StaticObject>();
	mTable->InitModel(GetObj<Texture>("table"), "Table", PrimitiveConfig::SQUARE);
	mTable->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mTable->LoadSaveData(sceneData);

	mJug = CreateObj<WaterJug>("Jug");
	mJug->InitPBR("Assets/Model/LabAssets/Jug.obj", "Jug");
	mJug->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mJug->LoadTex(pbrTexList);
	mJug->LoadSaveData(sceneData);


	mRedPotion = std::make_shared<Ingredient>();
	mRedPotion->InitModel("Assets/Model/Potion4.obj", "RedPotion", PrimitiveConfig::MULTI, { 1,1 });
	mRedPotion->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mRedPotion->LoadSaveData(sceneData);
	mRedPotion->SetPigmentColor(HSVtoRGB({ 0,1,0.5f }));

	mBluePotion = std::make_shared<Ingredient>();
	mBluePotion->InitModel("Assets/Model/Potion2.obj", "BluePotion", PrimitiveConfig::MULTI, { 1,1 });
	mBluePotion->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mBluePotion->LoadSaveData(sceneData);
	mBluePotion->SetPigmentColor(HSVtoRGB({240,1,0.5f}));

	mYellowPotion = std::make_shared<Ingredient>();
	mYellowPotion->InitModel("Assets/Model/Potion3.obj", "YellowPotion", PrimitiveConfig::MULTI, { 1,1 });
	mYellowPotion->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mYellowPotion->LoadSaveData(sceneData);
	mYellowPotion->SetPigmentColor(HSVtoRGB({ 60,1,0.5f }));

	mCandleLight1 = std::make_unique<CandleLight>();
	mCandleLight1->Init();
	mCandleLight1->LoadSaveData(sceneData, "CandleLight1");
	mCandleLight1->InitName("CandleLight1");

	mCandleLight2 = std::make_unique<CandleLight>();
	mCandleLight2->Init();
	mCandleLight2->LoadSaveData(sceneData, "CandleLight2");
	mCandleLight2->InitName("CandleLight2");

	//Set UI
	//reset button
	mResetButton = std::make_unique<UI_IconButtonReset>();
	mResetButton->Init("Assets/Texture/ButtonUI/IconBg.dds", "Assets/Texture/ButtonUI/CleanButtonTex.dds", "ResetButton");
	mResetButton->LoadSaveData(sceneData["ResetButton"]);
	mResetButton->SetPotion(mWater.get());

	//Charge button
	mRedIngredientButton = std::make_unique<UI_IngredientIconButton>();
	mRedIngredientButton->Init("Assets/Texture/ButtonUI/IconBg.dds", "Assets/Texture/ButtonUI/chargeTex.dds", "RedIngredientButton");
	mRedIngredientButton->SetIngredient(mRedPotion.get());
	mRedIngredientButton->LoadSaveData(sceneData["RedIngredientButton"]);

	mBlueIngredientButton = std::make_unique<UI_IngredientIconButton>();
	mBlueIngredientButton->Init("Assets/Texture/ButtonUI/IconBg.dds", "Assets/Texture/ButtonUI/chargeTex.dds", "BlueIngredientButton");
	mBlueIngredientButton->SetIngredient(mBluePotion.get());
	mBlueIngredientButton->LoadSaveData(sceneData["BlueIngredientButton"]);

	mYellowIngredientButton = std::make_unique<UI_IngredientIconButton>();
	mYellowIngredientButton->Init("Assets/Texture/ButtonUI/IconBg.dds", "Assets/Texture/ButtonUI/chargeTex.dds", "YellowIngredientButton");
	mYellowIngredientButton->SetIngredient(mYellowPotion.get());
	mYellowIngredientButton->LoadSaveData(sceneData["YellowIngredientButton"]);

	mGoldDisplay = CreateObj<UIGoldDisplay>("GoldDisplay");
	mGoldDisplay->Init(GetObj<Player>("player").get(), D2DUIConfig::FontSize::SEMI_SIZE);
	mGoldDisplay->LoadSceneData(sceneData["GoldDisplay"]);

	mSplash = std::make_unique<StaticObject>();
	mSplash->InitModel(GetObj<Texture>("splash"),"Splash",PrimitiveConfig::SQUARE,0);
	mSplash->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mSplash->LoadSaveData(sceneData);
	

	mPaper = std::make_unique<StaticObject>();
	mPaper->InitModel(GetObj<Texture>("paper4"), "MissionPaper", PrimitiveConfig::SQUARE, 0);
	mPaper->LoadDefShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mPaper->LoadSaveData(sceneData);

	ResultManager::Get()->SetUIGoldDisplay(mGoldDisplay.get());

	//Shadowに関するRenderTargetを作成
	InitShadowRenderTarget();

	IngredientManager::Get()->RegisterIngredient(mRedPotion.get(),PlayerConfig::RED);
	IngredientManager::Get()->RegisterIngredient(mBluePotion.get(), PlayerConfig::BLUE);
	IngredientManager::Get()->RegisterIngredient(mYellowPotion.get(),PlayerConfig::YELLOW);

	IngredientManager::Get()->InitPotionCapacity();

}

void ScenePotion::UnInit()
{
#ifdef _DEBUG
	json sceneData;

	//Save EnvironmentLight
	sceneData["EnvironmentLight"] = GetObj<DirLight>("EnvironmentLight")->SaveData();
	sceneData["ScenePotionWater"] = mWater->SaveData();
	sceneData["Pot"] = mPot->SaveData();
	sceneData["PotTop"] = mPotTop->SaveData();
	sceneData["Table"] = mTable->SaveData();
	sceneData["Jug"] = mJug->SaveData();

	sceneData["RedPotion"] = mRedPotion->SaveData();
	sceneData["BluePotion"] = mBluePotion->SaveData();
	sceneData["YellowPotion"] = mYellowPotion->SaveData();
	sceneData["CandleLight1"] = mCandleLight1->SaveData();
	sceneData["CandleLight2"] = mCandleLight2->SaveData();

	sceneData["RedIngredientButton"] = mRedIngredientButton->SaveData();
	sceneData["BlueIngredientButton"] = mBlueIngredientButton->SaveData();
	sceneData["YellowIngredientButton"] = mYellowIngredientButton->SaveData();

	sceneData["ResetButton"] = mResetButton->SaveData();
	sceneData["GoldDisplay"] = mGoldDisplay->SaveData();

	sceneData["Splash"] = mSplash->SaveData();
	sceneData["MissionPaper"] = mPaper->SaveData();

	SaveSceneFile("Assets/Data/SaveDat/scene_potion.json", sceneData);

#endif

	//もしここでゲーム終了の場合 ポーションキャパシティを取得
	Player* player = SceneManager::Get()->GetObj<Player>("player").get();
	player->SaveCapacitiesData(PlayerConfig::RED, mRedPotion->GetCapacity());
	player->SaveCapacitiesData(PlayerConfig::BLUE, mBluePotion->GetCapacity());
	player->SaveCapacitiesData(PlayerConfig::YELLOW, mYellowPotion->GetCapacity());



}

void ScenePotion::Update(float dt)
{
	TriggerListener();
	
	GameObjectUpdate(dt);

	if (ScreenFadeEffect::Get()->GetFadeIn())
	{
		if(mNextScene== SceneConfig::SceneIndex::SCENE_TITLE)
			SceneManager::Get()->SetMainScene("Lab");
	}

}

void ScenePotion::Draw()
{
	DrawWithShadow();

	
	mRedIngredientButton->Draw();
	mYellowIngredientButton->Draw();
	mBlueIngredientButton->Draw();

	mResetButton->Draw();
	mGoldDisplay->Draw();
	

}

void ScenePotion::DrawWithShadow()
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

	shadowPos = { mJug->GetPosition().x,0.0f, mJug->GetPosition().z };
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(
		XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation(shadowPos.x, shadowPos.y, shadowPos.z)));
	scaleBaseMatrix = XMMatrixScaling(mJug->GetScale().x, mJug->GetScale().y, mJug->GetScale().z);
	mat[0] = mat[0] * scaleBaseMatrix;
	LMatrix[0] = mat[0];
	GetObj<VertexShader>("VS_Primitives")->WriteShader(0, LMatrix);
	mJug->SetVertexShader(GetObj<VertexShader>("VS_Primitives").get());
	mJug->SetPixelShader(GetObj<PixelShader>("PS_WriteDepth").get());
	mJug->Draw();
	

	GameApp::SetDefaultRenderTarget();

	//Insert PointLight;
	Light::PointLight pl[2] = {
		mCandleLight1->GetPointLight(),
		mCandleLight2->GetPointLight()
	};

	
	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_Primitives")->WriteShader(1, pl);
	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_InterActiveObjectPBRModel")->WriteShader(1, pl);
	GetObj<PixelShader>("PS_Ingredient")->WriteShader(1, pl);
	GameApp::SetBlendState(RenderState::BSTransparent);
	mRedPotion->SwitchToDefShader();
	mRedPotion->Draw();
	mBluePotion->SwitchToDefShader();
	mBluePotion->Draw();
	mYellowPotion->SwitchToDefShader();
	mYellowPotion->Draw();
	GameApp::SetBlendState(nullptr);
	mJug->SwitchToDefShader();
	mJug->Draw();

	mPot->SwitchToDefShader();
	mPot->Draw();
	mPotTop->Draw();
	mWater->Draw();


	//Missionがある時だけ表示する
	if (MissionManager::Get()->HasCurrentMission())
	{
		mSplash->Draw();
		mPaper->Draw();
	}


	mTable->Draw();

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

void ScenePotion::InitShadowRenderTarget()
{	// レンダーターゲット作成
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

void ScenePotion::GameObjectUpdate(float dt)
{
	//Light
	mCandleLight1->Update(dt);
	mCandleLight2->Update(dt);

	//水
	mWater->Update(dt);

	//Table
	mPot->Update(dt);
	mPotTop->Update(dt);

	//Table
	mTable->Update(dt);

	//Potion
	mRedPotion->Update(dt);
	mBluePotion->Update(dt);
	mYellowPotion->Update(dt);

	//Dilute
	mJug->Update(dt);

	//UI
	mRedIngredientButton->Update(dt);
	mBlueIngredientButton->Update(dt);
	mYellowIngredientButton->Update(dt);
	mResetButton->Update(dt);
	mGoldDisplay->Update(dt);

	//Paper + Splash
	if (!MissionManager::Get()->HasCurrentMission())return;
	mSplash->SetDiffuseColor(MissionManager::Get()->GetCurrentMission().MissionColor);
	mSplash->Update(dt);
	mPaper->Update(dt);
}

void ScenePotion::TriggerListener()
{
	//Escapeでシーン切り替え
	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		if (ResultManager::Get()->GetActive())
		{
			ResultManager::Get()->SetActive(false);
			return;
		}

		SceneManager::Get()->SetSwitchSceneWithFade(true);
		mNextScene = SceneConfig::SceneIndex::SCENE_TITLE;
		return;
	}


	//リザルトが表示する場合　クリックで消す
	if(KInput::IsKeyTrigger(VK_LBUTTON))
	{
		//if (ResultManager::Get()->GetActive())ResultManager::Get()->SetActive(false);
	}

	
	


	
}

#include "ScenePotion.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "IngredientManager.h"
#include "InteractiveStaticObject.h"
#include "KInput.h"
#include "MissionManager.h"
#include "MissionSamplePaper.h"
#include "Potion.h"
#include "RenderState.h"
#include "SceneManager.h"
#include "SceneScore.h"
#include "Sprite.h"

using namespace DirectX;

void ScenePotion::Init()
{
	//Load Save Data
	json sceneData = LoadSceneData("Assets/Data/SaveDat/scene_potion.json");
	json uiData = LoadSceneData("Assets/Data/SaveDat/ui_config.json");
	//Init Potion Camera
	GetObj<FirstPersonCamera>("DefaultCamera")->SetPosition(0,10.57f,-4.51f);
	GetObj<FirstPersonCamera>("DefaultCamera")->mTransform.SetRotationInDegree(66.6f,0.f,0.f);
#ifdef NDEBUG
	GetObj<FirstPersonCamera>("DefaultCamera")->LockCamera();
#endif

	//Init Light
	GetObj<DirLight>("EnvironmentLight")->LoadSaveData(sceneData, "EnvironmentLight");
	//Init object
	mWater = CreateObj<Potion>("PotionSceneWater");
	mWater->LoadSaveData(sceneData, "ScenePotionWater");
	mWater->LoadShader(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	//mWater->SetTexture(GetObj<Texture>("water"));
	//mWater->ResetMaterial();
	mWater->SetWaterColor({ 0.4f,0.4f,0.4f,0.3f });
	mWater->SetTexture(nullptr);


	//Load Tex
	pbrTexList[PBRConfig::PBRTex::ALBEDO] = GetObj<Texture>("pbrAlbedo");
	pbrTexList[PBRConfig::PBRTex::METALLIC] = GetObj<Texture>("pbrMetallic");
	pbrTexList[PBRConfig::PBRTex::NORMAL] = GetObj<Texture>("pbrNormal");

	//Init Model & Objects
	mPot = CreateObj<InteractiveStaticObject>("pot");
	mPot->InitPBRModel("Assets/Model/LabAssets/Pot.obj", "pot");
	mPot->LoadShaderFile(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mPot->LoadTex(pbrTexList);
	mPot->LoadSaveData(sceneData, "Pot");
	mPot->DisableRimLightEffect();

	mTable = std::make_unique<StaticObject>();
	mTable->InitModel(GetObj<Texture>("table"), "Table", PrimitiveConfig::SQUARE);
	mTable->LoadShaderFile(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Primitives"));
	mTable->LoadSaveData(sceneData);

	mJug = CreateObj<WaterJug>("Jug");
	mJug->InitPBR("Assets/Model/LabAssets/Jug.obj", "Jug");
	mJug->LoadDefShader(GetObj<VertexShader>("VS_PBRModel"), GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
	mJug->LoadTex(pbrTexList);
	mJug->LoadSaveData(sceneData);

	mMissionPaper = std::make_unique<MissionSamplePaper>();
	mMissionPaper->Init(PrimitiveConfig::SQUARE, GetObj<Texture>("paper2"), "MissionPaper");
	mMissionPaper->LoadShaderFile(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_InteractiveObjectNormal"));
	mMissionPaper->LoadSaveData(sceneData);

	mMissionPaper->SetSampleColor(MissionManager::Get()->GetCurrentMissionInstance().GetMissionColor());

	//Set Water
	mWater->SetWaterBoilingState(WaterStateConfig::WaterBoilingState::STATE_BOILING);
	mWater->SetWaterState(WaterStateConfig::WaterState::STATE_RIPPLING);

	//Set UI
	mResetButton = std::make_unique<UI_Button>();
	mResetButton->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 1,1 }, GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	mResetButton->LoadSaveData(sceneData, "Reset");

	mChargeButton = std::make_unique<UI_Button>();
	mChargeButton->Init(UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE, nullptr, { 1,1 }, GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	mChargeButton->LoadSaveData(sceneData, "Charge");

	mGoldBar = std::make_unique<UIStackContainer>();
	mGoldBar->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mGoldBar->LoadBackgroundTex(nullptr, { 200,200 });
	mGoldBar->LoadFontTexture(GetObj<Texture>("UIFont_OCRA_Extend"), UITextOption::FONT_DEFAULT_SIZE);
	mGoldBar->LoadSaveData(uiData, "GoldBar");


	mRedPotion = std::make_shared<Ingredient>();
	mRedPotion->InitModel("Assets/Model/Potion4.obj", "RedPotion", PrimitiveConfig::MULTI, { 1,1 });
	mRedPotion->LoadShaderFile(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mRedPotion->LoadSaveData(sceneData);

	mBluePotion = std::make_shared<Ingredient>();
	mBluePotion->InitModel("Assets/Model/Potion2.obj", "BluePotion", PrimitiveConfig::MULTI, { 1,1 });
	mBluePotion->LoadShaderFile(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mBluePotion->LoadSaveData(sceneData);


	mYellowPotion = std::make_shared<Ingredient>();
	mYellowPotion->InitModel("Assets/Model/Potion3.obj", "YellowPotion", PrimitiveConfig::MULTI, { 1,1 });
	mYellowPotion->LoadShaderFile(GetObj<VertexShader>("VS_Primitives"), GetObj<PixelShader>("PS_Ingredient"));
	mYellowPotion->LoadSaveData(sceneData);

	mCandleLight = std::make_unique<CandleLight>();
	mCandleLight->Init();
	mCandleLight->LoadSaveData(sceneData, "CandleLight");

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
	sceneData["Table"] = mTable->SaveData();
	sceneData["MissionPaper"] = mMissionPaper->SaveData();
	sceneData["Jug"] = mJug->SaveData();

	sceneData["Reset"] = mResetButton->SaveData ();
	sceneData["Charge"] =mChargeButton->SaveData();

	sceneData["RedPotion"] = mRedPotion->SaveData();
	sceneData["BluePotion"] = mBluePotion->SaveData();
	sceneData["YellowPotion"] = mYellowPotion->SaveData();
	sceneData["CandleLight"] = mCandleLight->SaveData();

	SaveSceneFile("Assets/Data/SaveDat/scene_potion.json", sceneData);

#endif

}

void ScenePotion::Update(float dt)
{
	TriggerListener();
	GameObjectUpdate(dt);

	//Save capacity to player file
	IngredientManager::Get()->UpdateCapacityData();

}

void ScenePotion::Draw()
{
	DrawWithShadow();

	mResetButton->Draw();
	mChargeButton->Draw();
	mGoldBar->Draw();

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
		mCandleLight->GetPointLight(),
		{}
	};

	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_Primitives")->WriteShader(1, pl);
	//Set PointLight to pbr shader
	GetObj<PixelShader>("PS_InterActiveObjectPBRModel")->WriteShader(1, pl);
	GetObj<PixelShader>("PS_Ingredient")->WriteShader(1, pl);
	mTable->SwitchToDefShader();
	mTable->Draw();
	mRedPotion->SwitchToDefShader();
	mRedPotion->Draw();
	mBluePotion->SwitchToDefShader();
	mBluePotion->Draw();
	mYellowPotion->SwitchToDefShader();
	mYellowPotion->Draw();
	mJug->SwitchToDefShader();
	mJug->Draw();
	mMissionPaper->Draw();
	//mMissionPaper->Draw();

	GameApp::SetBlendState(RenderState::BSTransparent);
	mPot->SwitchToDefShader();
	mPot->Draw();
	mWater->Draw();


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
	mWater->Update(dt);
	mPot->Update(dt);
	mTable->Update(dt);
	mRedPotion->Update(dt);
	mBluePotion->Update(dt);
	mYellowPotion->Update(dt);
	mJug->Update(dt);
	mMissionPaper->Update(dt);

	mResetButton->Update();
	mChargeButton->Update();

	mGoldBar->SetText(GetObj<Player>("player")->GetPlayerGold().c_str());
	mGoldBar->Update();

	mCandleLight->Update(dt);
}

void ScenePotion::TriggerListener()
{
	if (KInput::IsKeyTrigger(VK_ESCAPE))
	{
		if (isSubScene)
		{
			RemoveSubScene();//Remove SubScene
			isSubScene = false;
		}

		else
		{
			SceneManager::Get()->SetSwitchScene(true);
			SceneManager::Get()->SetMainScene("Lab");
		}
		return;
	}

	if(mResetButton->isTrigger())
	{
		mWater->ResetMaterial();
	}

	if(mChargeButton->isTrigger())
	{
		IngredientManager::Get()->ChargeAllIngredient();
	}

	if(mMissionPaper->GetClicked())
	{
		isSubScene = true;
		//AddSubScene<SceneScore>();
	}

}

#include "ResultManager.h"

#include "GameApp.h"
#include "Potion.h"
#include "RenderState.h"

void ResultManager::Init()
{
	mResult = std::make_unique<D2D_UIStackContainer>();
	mResult->Init(D2DUIConfig::UIShape::RECT, D2DUIConfig::FontSize::TITLE_SIZE, "Result");
	mResult->SetUIState(D2DUIConfig::STATE_USE_FONT);

	mResultBackGround = std::make_unique<UI_Square>();
	mResultBackGround->Init(nullptr, 20);
	mResultBackGround->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_DefaultUI"), SceneManager::Get()->GetObj<PixelShader>("PS_RoundGradation"));
	mResultBackGround->SetPosition(mResult->GetPosition());
	mResultBackGround->SetScale(400, 200);
	mResultBackGround->SetPosZ(0.2f);
	mResultBackGround->SetDiffuseColor({ 0.8f,0.8f,0.8f,1.0f });
	mShadowRadius = 2.f;
	mChangeRange = 0.005f;

}

void ResultManager::Update(float dt)
{
	if (!isActive)return;

	//指令を受けて、結果を計算する
	if(!isFinishIsGenerateResult)
	{
		GenerateCurrentResult();
	}
	//結果を渡す
	mResult->SetText(mResultContents.c_str());
	mResult->Update(dt);

	//結果が浮かび上がったかつリザルトが出てきた
	if (!mResult->GetIsEmerging() && isReadyToSetIncreaseMoney)
	{
		//賞金を上げる
		mGoldDisplay->SetIncreaseFunction(1.0f, (int)increaseGoldNum);
		isReadyToSetIncreaseMoney = false;
	}

	mResultBackGround->Update();
}

void ResultManager::Draw()
{
	if (!isActive)return;

	//結果がある時のみ表示する
	if(isFinishIsGenerateResult)
	{
		GameApp::SetBlendState(RenderState::BSAlphaWeightedAdditive);
		mResultBackGround->GetDefPS()->WriteShader(1, &mShadowRadius);
		mResultBackGround->Draw();
		GameApp::SetBlendState(nullptr);
		mResult->Draw();
	}
	
}

void ResultManager::ResultGenerator()
{
	
}

void ResultManager::GenerateCurrentResult()
{
	Potion* potion = SceneManager::Get()->GetObj<Potion>("PotionSceneWater").get();

	if (!MissionManager::Get()->HasCurrentMission())
	{
		//現在ミッションない場合、結果表示しない
		return;
	}

	HSV potionColor = RGBtoHSV(potion->GetPotionColor());
	HSV missionColor = RGBtoHSV(MissionManager::Get()->GetCurrentMission().MissionColor);
	//最小値・絶対値
	float hueDiff = std::min(fabs(potionColor.hue - missionColor.hue), 360.f - fabs(potionColor.hue - missionColor.hue));
	float saturationDiff = fabs(potionColor.saturation - missionColor.saturation);
	float valueDiff = fabs(potionColor.value - missionColor.value);

	float resultDiff = (hueDiff / 360.f) * ResultConfig::HUE_WEIGHT + saturationDiff * ResultConfig::SATURATION_WEIGHT + valueDiff * ResultConfig::VALUE_WEIGHT;

	if (resultDiff < 0.1f)
		mResultContents = "S";
	else if (resultDiff >= 0.1f && resultDiff < 0.25f)
		mResultContents = "A";
	else if (resultDiff >= 0.25f && resultDiff < 0.5f)
		mResultContents = "B";
	else
		mResultContents = "C";

	//リザルトが浮かぶ
	mResult->InitEmergingFunc(1.0f);
	increaseGoldNum = MissionManager::Get()->GetCurrentMission().MissionReward;
	isReadyToSetIncreaseMoney = true;

	//任務完成
	MissionManager::Get()->CompleteCurrentMission();
	isFinishIsGenerateResult = true;
}

json ResultManager::SaveData()
{
	json data;
	data["ResultUI"] = mResult->SaveData();
	return data;
}

void ResultManager::LoadSaveData(json data)
{
	mResult->LoadSaveData(data["ResultUI"]);
	mResultBackGround->SetPosition(mResult->GetPosition().x, mResult->GetPosition().y - mResult->GetScale().y / 2.f);


}

void ResultManager::SetUIGoldDisplay(UIGoldDisplay* display)
{
	mGoldDisplay = nullptr;
	mGoldDisplay = display;
}


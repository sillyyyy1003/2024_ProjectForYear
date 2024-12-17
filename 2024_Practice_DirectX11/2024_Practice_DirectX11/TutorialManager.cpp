#include "TutorialManager.h"
#include "DebugLog.h"
#include "GameApp.h"
#include "KInput.h"
#include "RenderState.h"
#include "SceneManager.h"
#include "ScreenOverlay.h"


TutorialManager::TutorialManager()
{
}


void TutorialManager::Init(json sceneData)
{
    //FirstPhase;
    mFirstPhaseBackground = std::make_unique<UISquare>();
    mFirstPhasePaper = std::make_unique<UISquare>();
    mFirstPhaseText = std::make_unique<D2D_UIStackContainer>();
    mFirstPhaseSkipButton = std::make_unique<D2D_UIStackContainer>();
    mFirstPhaseFrontFade = std::make_unique<UISquare>();

    mFirstPhaseBackground->Init(SceneManager::Get()->GetObj<Texture>("BlackOverlay"), DirectX::XMINT2(1, 1));
    mFirstPhaseBackground->LoadSaveData(sceneData["FirstPhaseBackGround"]);

    mFirstPhasePaper->Init(SceneManager::Get()->GetObj<Texture>("paper3"), DirectX::XMINT2(1, 1));
    mFirstPhasePaper->LoadSaveData(sceneData["FirstPhasePaper"]);

    mFirstPhaseText->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "FirstPhaseText");
    mFirstPhaseText->SetUIState(D2DUIConfig::STATE_USE_FONT);
    mFirstPhaseText->LoadSaveData(sceneData["FirstPhaseText"]);

    mFirstPhaseSkipButton->Init(D2DUIConfig::UIShape::ROUNDED_RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "FirstPhaseSkipButton");
    mFirstPhaseSkipButton->SetUIState(D2DUIConfig::STATE_USE_FONT);
    mFirstPhaseSkipButton->LoadSaveData(sceneData["FirstPhaseSkipButton"]);
    
    mFirstPhaseFrontFade->Init(SceneManager::Get()->GetObj<Texture>("Fade"), DirectX::XMINT2(1, 1));
    mFirstPhaseFrontFade->LoadSaveData(sceneData["FirstPhaseFade"]);

    posY = mFirstPhaseSkipButton->GetPosition().y;
}

void TutorialManager::Update(float dt)
{
    if (!isUseTutorial)return;
      
	switch (mTutorialPhaseState)
	{
	case Tutorial::Tutorial_00_None:
        CompleteCurrentPhase();
		break;
	case Tutorial::Tutorial_01_Intro:
#ifdef _DEBUG
        DebugLog::Log("Phase Intro");
#endif
        isSceneFreeze = true;

        //Fade
        if(mFadeParam>=0)
        {
            mFadeParam -= dt * 0.5f * ScreenOverlayConfig::FADE_SPEED;
        }

        mFirstPhaseFrontFade->SetTransparency(mFadeParam);

        //Object Update
        mFirstPhaseBackground->Update();
        mFirstPhasePaper->Update();
        mFirstPhaseText->Update(dt);
        mFirstPhaseSkipButton->Update(dt);
        mFirstPhaseFrontFade->Update();

        mAccumulateTime += dt * 5.f;
        mFirstPhaseSkipButton->SetPositionY(posY + sin(mAccumulateTime) * mAmplitude);

        //first skip button levitating
        if(mFadeParam<=0&&KInput::IsKeyTrigger(VK_LBUTTON))CompleteCurrentPhase();
        
        //Trigger
        break;
    case Tutorial::Tutorial_02_Workshop:
#ifdef _DEBUG
        DebugLog::Log("Phase Workshop");
#endif

        break;
    case Tutorial::Tutorial_03_BuyMaterial:
#ifdef _DEBUG
        DebugLog::Log("Phase BuyMaterial");
#endif

        break;
    case Tutorial::Tutorial_04_FirstCraft:
#ifdef _DEBUG
        DebugLog::Log("Phase FirstCraft");
#endif

        break;
    case Tutorial::Tutorial_05_ActionIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase ActionIntro");
#endif

        break;
    case Tutorial::Tutorial_06_ResetIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase ResetIntro");
#endif

        break;
    case Tutorial::Tutorial_07_Supplement:
#ifdef _DEBUG
        DebugLog::Log("Phase Supplement");
#endif

        break;
    case Tutorial::Tutorial_08_SellIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase SellIntro");
#endif

        break;
    case Tutorial::Tutorial_09_RateIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase RateIntro");
#endif

        break;
    case Tutorial::Tutorial_10_LevelUpIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase LevelUpIntro");
#endif

        break;
    default:
        return;
	}


    MoveToNextPhase();

}

void TutorialManager::Draw()
{

    if (!isUseTutorial)return;
    switch (mTutorialPhaseState)
    {
    default:
    case Tutorial::Tutorial_01_Intro:
#ifdef _DEBUG
        DebugLog::Log("Phase Intro");
#endif
        DrawIntroPhase();
        break;
    case Tutorial::Tutorial_02_Workshop:
#ifdef _DEBUG
        DebugLog::Log("Phase Workshop");
#endif

        break;
    case Tutorial::Tutorial_03_BuyMaterial:
#ifdef _DEBUG
        DebugLog::Log("Phase BuyMaterial");
#endif

        break;
    case Tutorial::Tutorial_04_FirstCraft:
#ifdef _DEBUG
        DebugLog::Log("Phase FirstCraft");
#endif

        break;
    case Tutorial::Tutorial_05_ActionIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase ActionIntro");
#endif

        break;
    case Tutorial::Tutorial_06_ResetIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase ResetIntro");
#endif

        break;
    case Tutorial::Tutorial_07_Supplement:
#ifdef _DEBUG
        DebugLog::Log("Phase Supplement");
#endif

        break;
    case Tutorial::Tutorial_08_SellIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase SellIntro");
#endif

        break;
    case Tutorial::Tutorial_09_RateIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase RateIntro");
#endif

        break;
    case Tutorial::Tutorial_10_LevelUpIntro:
#ifdef _DEBUG
        DebugLog::Log("Phase LevelUpIntro");
#endif

        break;
    }

}

void TutorialManager::CompleteCurrentPhase()
{
    mCurrentPhaseCompleted = true;
}

void TutorialManager::MoveToNextPhase()
{
    if (!mCurrentPhaseCompleted)return;

    switch (mTutorialPhaseState)
    {
    case Tutorial::Tutorial_00_None:
        mTutorialPhaseState = 0;
        mTutorialPhaseState |= Tutorial::Tutorial_01_Intro;
        posY = mFirstPhaseSkipButton->GetPosition().y;
        break;
    case Tutorial::Tutorial_01_Intro:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase Intro");
#endif
        //Reset all current resource
        mFirstPhaseSkipButton->SetPositionY(posY);
        mTutorialPhaseState = 0;
        mTutorialPhaseState |= Tutorial::Tutorial_02_Workshop;
        isSceneFreeze = false;

        break;
    case Tutorial::Tutorial_02_Workshop:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase Workshop");
#endif

        break;
    case Tutorial::Tutorial_03_BuyMaterial:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase BuyMaterial");
#endif

        break;
    case Tutorial::Tutorial_04_FirstCraft:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase FirstCraft");
#endif

        break;
    case Tutorial::Tutorial_05_ActionIntro:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase ActionIntro");
#endif

        break;
    case Tutorial::Tutorial_06_ResetIntro:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase ResetIntro");
#endif

        break;
    case Tutorial::Tutorial_07_Supplement:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase Supplement");
#endif

        break;
    case Tutorial::Tutorial_08_SellIntro:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase SellIntro");
#endif

        break;
    case Tutorial::Tutorial_09_RateIntro:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase RateIntro");
#endif

        break;
    case Tutorial::Tutorial_10_LevelUpIntro:
#ifdef _DEBUG
        DebugLog::Log("Complete Phase LevelUpIntro");
#endif

        break;
        default:
            return;
    }

    mCurrentPhaseCompleted = false;
   
}

void TutorialManager::DrawIntroPhase()
{
    //ScreenOverlay
    GameApp::SetBlendState(RenderState::BSMulti);
    mFirstPhaseBackground->Draw();
    GameApp::SetBlendState(nullptr);
    //Draw Paper Texture
    mFirstPhasePaper->Draw();
    //Draw FontText
    mFirstPhaseText->Draw();
    //Skip Button
    mFirstPhaseSkipButton->Draw();
    //Fade
    GameApp::SetBlendState(RenderState::BSTransparent);
	mFirstPhaseFrontFade->Draw();
    GameApp::SetBlendState(nullptr);

}


json TutorialManager::UnInit()
{
    json sceneData;
    sceneData["FirstPhaseBackGround"] = mFirstPhaseBackground->SaveData();
    sceneData["FirstPhasePaper"] = mFirstPhasePaper->SaveData();
    sceneData["FirstPhaseText"] = mFirstPhaseText->SaveData();
    sceneData["FirstPhaseSkipButton"] = mFirstPhaseSkipButton->SaveData();
    sceneData["FirstPhaseFade"] = mFirstPhaseFrontFade->SaveData();
    return sceneData;
}



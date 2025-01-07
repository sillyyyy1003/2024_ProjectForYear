#include "MissionPaper.h"

#include "KInput.h"
#include "MissionManager.h"

MissionPaper::MissionPaper()
{
}

void MissionPaper::InitMissionPaper()
{
	mPaperMesh = std::make_unique<Square>();
	mSplash = std::make_unique<Square>();
	mText = std::make_unique<Square>();
	m2DFont = std::make_unique<D2D_UIStackContainer>();

	mPaperMesh->Init(SceneManager::Get()->GetObj<Texture>("paper4"),0);
	mSplash->Init(SceneManager::Get()->GetObj<Texture>("paper4"), 0);
	mText->Init(SceneManager::Get()->GetObj<Texture>("paper4"), 0);

	m2DFont->Init(D2DUIConfig::UIShape::RECT, D2DUIConfig::FontSize::NORMAL_SIZE, "Paper2DFont");
	m2DFont->SetUIState(D2DUIConfig::STATE_USE_FONT);

	mPaperMesh->LoadDefShader();
	mSplash->LoadDefShader();
	mText->LoadDefShader();

	mPaperMesh->SetScale({ 100.f,100.f });
	mSplash->SetScale({ 100.f,100.f });
}

void MissionPaper::Update(float dt)
{

	PreUpdate(dt);

	GameUpdate(dt);

	if(KInput::IsKeyTrigger(VK_SPACE))
	{
		static_cast<Paper::PaperState>(mState << 1);
	}

	if(mState > Paper::STATE_DISAPPEAR)
	{
		mState = Paper::STATE_FALLING;
	}
}

void MissionPaper::Draw()
{
	switch (mState)
	{
	case Paper::STATE_DEFAULT://机の上に置いてある状態
		mPaperMesh->Draw();
		mSplash->Draw();
		mText->Draw();
		break;
	case Paper::STATE_FALLING:	//演出用
		mPaperMesh->Draw();
		break;
	case Paper::STATE_DISAPPEAR:	//Missionがない時
		break;
	case Paper::STATE_ZOOM:	//Mission内容を表示する
		//Draw3D BackGround
		mPaperMesh->Draw();
		mSplash->Draw();
		//Draw Font
		
		break;
	default:
		return;
	}
}

void MissionPaper::LoadSaveData(json data)
{
}

json MissionPaper::SaveData()
{
	return 0;
}

void MissionPaper::PreUpdate(float dt)
{
	mPaperMesh->Update(dt);
	mSplash->Update(dt);
	mText->Update(dt);
	m2DFont->Update(dt);
}

void MissionPaper::GameUpdate(float dt)
{
	switch (mState)
	{
	case Paper::STATE_DEFAULT:
		break;
	case Paper::STATE_FALLING:
		break;
	case Paper::STATE_DISAPPEAR:
		break;
	case Paper::STATE_ZOOM:
		break;
	default:
		return;
	}
}

void MissionPaper::LateUpdate(float dt)
{
}

void MissionPaper::SetState(Paper::PaperState state)
{
	//Remove former State
	mState = 0;
	//Set current State
	mState |= state;
}

void MissionPaper::SetPosition(DirectX::XMFLOAT3 position)
{
	mPosition = position;
}

void MissionPaper::SetRotation(DirectX::XMFLOAT3 rotation)
{
	mRotation = rotation;
}

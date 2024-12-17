#include "MissionPaper.h"
#include "DirLight.h"
#include "GameApp.h"
#include "KInput.h"
#include "MissionManager.h"
#include "RenderState.h"


enum ObjectState
{
	STATE_NONE = 0,
	STATE_HOVER = 1,
	STATE_CLICK = 2,
	STATE_MAX = 99
};
using namespace DirectX;
using namespace DirectX::SimpleMath;

MissionPaper::MissionPaper()
{
}

void MissionPaper::Init(const std::shared_ptr<Texture>& tex, const char* _objName)
{
	mPaper = std::make_unique<Square>();
	mPaper->Init(tex,0);
	mPaper->SetRotation(-90, 0, 0);

	InitCollider();
}

void MissionPaper::Init(const char* filePath, const char* _objName)
{
	mPaper = std::make_unique<Square>();
	mPaper->Init(filePath, 0);
	mPaper->SetRotation(-90, 0, 0);
	InitCollider();
}

void MissionPaper::InitMission(float reward,DirectX::XMFLOAT4 targetColor)
{
	mMission = std::make_unique<Mission>();
	mMission->SetReward(reward);
	mMission->SetMissionColor(targetColor);
}

void MissionPaper::Update(float dt)
{
	PreUpdate(dt);

	GameUpdate(dt);

	LateUpdate(dt);
}

void MissionPaper::Draw()
{

	mPaper->GetDefPS()->WriteShader(2, &mEffect);
	mPaper->Draw();

}

void MissionPaper::SetPosition(DirectX::XMFLOAT3 pos)
{
	mPaper->SetPosition(pos);
	NotifyModelStateChangeListener();
}

void MissionPaper::SetScale(const DirectX::XMFLOAT2& scale)
{
	mPaper->SetScale(scale);
	NotifyModelStateChangeListener();
}

bool MissionPaper::GetClicked()
{
	if(isClicked)
	{
		//reset trigger
		isClicked = false;
		return true;
	}
	else
	{
		return false;
	}
}

void MissionPaper::LoadDefShader(const std::shared_ptr<VertexShader>& mVS, const std::shared_ptr<PixelShader>& mPS)
{
	mPaper->LoadDefShader(mVS,mPS);

}

void MissionPaper::PreUpdate(float dt)
{
	if (GetModelStateChange())
	{
		UpdateCollider();
		ClearModelStateChangeListener();
	}

	int prev = static_cast<ObjectState>(mObjectState);

	switch (prev)
	{
	default:
	case ObjectState::STATE_NONE:
	{
		//現在のカメラを取得
		CameraBase* camera = GameApp::GetCurrentCamera();
		//マウスの位置スクリーン座標を取得
		POINT mousePos;
		GetCursorPos(&mousePos);
		//カメラからマウス位置の方向ベクトルを取得
		XMVECTOR rayDir = camera->ScreenPointToRay(mousePos);
		//カメラの位置を取得
		XMFLOAT3 camPos = camera->GetPos();
		XMVECTOR startPos = XMLoadFloat3(&camPos);
		float distance = 0;
		GetCursorPos(&mousePos);

		if (mCollider->Interacts(startPos, rayDir, distance))
		{
			mObjectState = static_cast<int>(ObjectState::STATE_HOVER);
		}
	}
	break;
	case ObjectState::STATE_HOVER:
	{
		//現在のカメラを取得
		CameraBase* camera = GameApp::GetCurrentCamera();
		//マウスの位置スクリーン座標を取得
		POINT mousePos;
		GetCursorPos(&mousePos);
		//カメラからマウス位置の方向ベクトルを取得
		XMVECTOR rayDir = camera->ScreenPointToRay(mousePos);
		//カメラの位置を取得
		XMFLOAT3 camPos = camera->GetPos();
		XMVECTOR startPos = XMLoadFloat3(&camPos);
		float distance = 0;
		GetCursorPos(&mousePos);

		if (mCollider->Interacts(startPos, rayDir, distance))
		{
			if (KInput::IsKeyTrigger(VK_LBUTTON))
			{
				mObjectState = ObjectState::STATE_CLICK;
			}
		}
		else
		{
			mObjectState = ObjectState::STATE_NONE;
		}
	}
	break;

	case ObjectState::STATE_CLICK:
	{
		//現在のカメラを取得
		CameraBase* camera = GameApp::GetCurrentCamera();
		//マウスの位置スクリーン座標を取得
		POINT mousePos;
		GetCursorPos(&mousePos);

		//カメラからマウス位置の方向ベクトルを取得
		XMVECTOR rayDir = camera->ScreenPointToRay(mousePos);
		//カメラの位置を取得
		XMFLOAT3 camPos = camera->GetPos();
		XMVECTOR startPos = XMLoadFloat3(&camPos);
		float distance = 0;

		if (mCollider->Interacts(startPos, rayDir, distance))
		{
			mObjectState = static_cast<int>(ObjectState::STATE_HOVER);
		}
		else
		{
			mObjectState = static_cast<int>(ObjectState::STATE_NONE);
		}
	
	}
	break;
	}

}

void MissionPaper::GameUpdate(float dt)
{
	switch (mObjectState)
	{
	default:
	case ObjectState::STATE_NONE:
		OnStateNone();
		break;
	case ObjectState::STATE_HOVER:
		OnStateHover();
		break;
	case ObjectState::STATE_CLICK:
		OnStateClicked();
		break;
	}
}

void MissionPaper::LateUpdate(float dt)
{
	mPaper->Update(dt);
}

void MissionPaper::InitCollider()
{
	mCollider = std::make_unique<BoxCollider>();

	mCollider->SetCenter(mPaper->mTransform.GetPosition());
	Vector3 boxSize = mPaper->mTransform.GetScale();
	boxSize /= 2.0f;
	mCollider->SetExtents(boxSize);

}

void MissionPaper::NotifyModelStateChangeListener()
{
	isModelStateChange = true;
}

void MissionPaper::ClearModelStateChangeListener()
{
	isModelStateChange = false;
}

void MissionPaper::OnStateNone()
{
	//Reset Click
	isClicked = false;

	Color ambient = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight")->GetAmbient();
	mPaper->SetAmbient(ambient * 0.75f);
	mEffect.rimIntensity = 0.0f;
}

void MissionPaper::OnStateHover()
{
	//Reset Click
	isClicked = false;

	Color ambient = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight")->GetAmbient();
	mPaper->SetAmbient(ambient);
	mEffect.rimIntensity = 0.4f;

}

void MissionPaper::OnStateClicked()
{
	// Output Trigger
	isClicked = true;

	Color ambient = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight")->GetAmbient();
	mPaper->SetAmbient(ambient * 0.75f);
	mEffect.rimIntensity = 0.4f;
	//todo:任务点击要具有排他性!
	mMission->RegisterAsCheckMission();
}

void MissionPaper::UpdateCollider()
{
	//Update extents
	Vector3 boxSize = mPaper->GetScale();
	boxSize /= 2.0f;
	mCollider->SetExtents(boxSize);

	//Update center
	mCollider->SetCenter(mPaper->GetPosition());

	//Update orientation
	mCollider->SetOrientation(mPaper->mTransform.GetQuaternion());

}
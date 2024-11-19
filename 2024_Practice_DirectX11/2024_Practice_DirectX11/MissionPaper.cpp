#include "MissionPaper.h"
#include "GampApp.h"
#include "KInput.h"

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

void MissionPaper::Init(std::shared_ptr<Texture> tex, const char* _objName)
{
	mModel = std::make_unique<Tile>();
	mModel->Init(tex, 50);
	InitCollider();

	mModel->LoadDefShader("Assets/Shader/VS_Primitives.cso", "Assets/Shader/PS_Object.cso");
}

void MissionPaper::Init(const char* filePath, const char* _objName)
{
	mModel = std::make_unique<Tile>();
	mModel->Init(filePath, 50);
	InitCollider();
}

void MissionPaper::Update(float dt)
{
	PreUpdate(dt);

	GameUpdate(dt);

	LateUpdate(dt);

}

void MissionPaper::Draw()
{
	mModel->Draw();
}

void MissionPaper::SetPosition(DirectX::XMFLOAT3 pos)
{
	mModel->SetPosition(pos);
	NotifyModelStateChangeListener();
}

void MissionPaper::SetScale(const DirectX::XMFLOAT2& scale)
{
	mModel->SetScale(scale);
	NotifyModelStateChangeListener();
}

void MissionPaper::PreUpdate(float dt)
{
	if (GetModelStateChange())
	{
		UpdateCollider();
		ClearModelStateChangeListener();
	}

	int prev = static_cast<ObjectState>(mObjectState);

	switch(prev)
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
	mModel->Update(dt);
	mModel->GetDefPS()->WriteShader(3, &mEffect);
}

void MissionPaper::InitCollider()
{
	mCollider = std::make_unique<BoxCollider>();

	mCollider->SetCenter(mModel->mTransform.GetPosition());
	Vector3 boxSize = mModel->mTransform.GetScale();
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
	mEffect = { 0.85f,0.85f,0.85f,1.0f };
}

void MissionPaper::OnStateHover()
{
	mEffect = { 1.0f,1.0f,1.0f,1.0f };
}

void MissionPaper::OnStateClicked()
{
	// Output Trigger
	isClicked = true;
	mEffect = { 1.0f,1.0f,1.0f,1.0f };
}

void MissionPaper::UpdateCollider()
{
	//Update extents
	Vector3 boxSize = mModel->GetScale();
	boxSize /= 2.0f;
	mCollider->SetExtents(boxSize);

	//Update center
	mCollider->SetCenter(mModel->GetPosition());

	//Update orientation
	mCollider->SetOrientation(mModel->mTransform.GetQuaternion());

}

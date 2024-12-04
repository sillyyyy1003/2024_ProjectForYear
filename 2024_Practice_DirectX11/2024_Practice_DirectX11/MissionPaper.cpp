#include "MissionPaper.h"
#include "DirLight.h"
#include "GameApp.h"
#include "KInput.h"
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
	mModel = std::make_unique<Square>();
	mModel->Init(tex,0);
	mModel->SetRotation(-90, 0, 0);
	InitCollider();
}

void MissionPaper::Init(const char* filePath, const char* _objName)
{
	mModel = std::make_unique<Square>();
	mModel->Init(filePath, 0);
	mModel->SetRotation(-90, 0, 0);
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
	mModel->SwitchToDefShader();
	mModel->GetDefPS()->WriteShader(2, &mEffect);
	mModel->Draw();

	//DrawOutline

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

bool MissionPaper::GetClicked()
{
	return this->isClicked;
}

void MissionPaper::LoadDefShader(const std::shared_ptr<VertexShader>& mVS, const std::shared_ptr<PixelShader>& mPS)
{
	mModel->LoadDefShader(mVS,mPS);
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
		//���݂̃J�������擾
		CameraBase* camera = GameApp::GetCurrentCamera();
		//�}�E�X�̈ʒu�X�N���[�����W���擾
		POINT mousePos;
		GetCursorPos(&mousePos);
		//�J��������}�E�X�ʒu�̕����x�N�g�����擾
		XMVECTOR rayDir = camera->ScreenPointToRay(mousePos);
		//�J�����̈ʒu���擾
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
		//���݂̃J�������擾
		CameraBase* camera = GameApp::GetCurrentCamera();
		//�}�E�X�̈ʒu�X�N���[�����W���擾
		POINT mousePos;
		GetCursorPos(&mousePos);
		//�J��������}�E�X�ʒu�̕����x�N�g�����擾
		XMVECTOR rayDir = camera->ScreenPointToRay(mousePos);
		//�J�����̈ʒu���擾
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
		//���݂̃J�������擾
		CameraBase* camera = GameApp::GetCurrentCamera();
		//�}�E�X�̈ʒu�X�N���[�����W���擾
		POINT mousePos;
		GetCursorPos(&mousePos);

		//�J��������}�E�X�ʒu�̕����x�N�g�����擾
		XMVECTOR rayDir = camera->ScreenPointToRay(mousePos);
		//�J�����̈ʒu���擾
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
	//Reset Click
	isClicked = false;

	Color ambient = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight")->GetAmbient();
	mModel->SetAmbient(ambient * 0.75f);
	mEffect.rimIntensity = 0.0f;
}

void MissionPaper::OnStateHover()
{
	//Reset Click
	isClicked = false;

	Color ambient = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight")->GetAmbient();
	mModel->SetAmbient(ambient);
	mEffect.rimIntensity = 0.4f;

}

void MissionPaper::OnStateClicked()
{
	// Output Trigger
	isClicked = true;

	Color ambient = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight")->GetAmbient();
	mModel->SetAmbient(ambient * 0.75f);
	mEffect.rimIntensity = 0.4f;
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
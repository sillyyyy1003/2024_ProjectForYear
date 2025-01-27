#include "InteractiveMovableObject.h"
#include "GameApp.h"
#include "KInput.h"
#include "Model.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
enum ObjectState
{
	STATE_NONE = 0,
	STATE_HOVER = 1,
	STATE_DRAG = 3,
	STATE_RELEASE = 4, 
	STATE_MAX = 99
};

InteractiveMovableObject::InteractiveMovableObject()
{

}

void InteractiveMovableObject::InitModel(const char* filePath, const char* _objName,
	PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit)
{
	StaticObject::InitModel(filePath, _objName, _kind, _UVSplit);
	InitCollider();
}

void InteractiveMovableObject::InitPBR(const char* filePath, const char* _objName)
{
	StaticObject::InitPBR(filePath, _objName);
	InitCollider();
}

void InteractiveMovableObject::Update(float dt)
{
	PreUpdate(dt);

	GameUpdate(dt);

	LateUpdate(dt);

}

void InteractiveMovableObject::Draw()
{
	mModel->GetDefPS()->WriteShader(2, &mEffect);
	mModel->Draw();

#ifdef _DEBUG
	if (isShowCollider)
		mDebugColliderMesh->Draw();
#endif

}

bool InteractiveMovableObject::GetClicked()
{
	if (isClicked)
	{
		isClicked = false;
		return true;
	}
	else
	{
		return false;
	}
}

void InteractiveMovableObject::SetModelPosition(DirectX::XMFLOAT3 pos)
{
	Vector3 modelPos = mModel->GetPosition();
	if (modelPos != pos)
		NotifyModelStateChangeListener();
	mModel->mTransform.SetPosition(pos);
}

void InteractiveMovableObject::SetModelRotation(DirectX::XMFLOAT3 rot)
{
	Vector3 modelRot = mModel->GetRotation();
	if (modelRot != rot)
	{
		NotifyModelStateChangeListener();
		mModel->mTransform.SetRotationInDegree(rot);
	}

}

void InteractiveMovableObject::SetModelScale(DirectX::XMFLOAT3 scale)
{
	Vector3 modelScale = mModel->GetScale();

	if (modelScale != scale)
		NotifyModelStateChangeListener();

	mModel->mTransform.SetScale(scale);
}

void InteractiveMovableObject::SetModelPosition(const float* position)
{
	Vector3 modelPos = mModel->GetPosition();
	if (modelPos != Vector3(position[0], position[1], position[2]))
		NotifyModelStateChangeListener();

	mModel->SetPosition(position);
}

void InteractiveMovableObject::SetModelRotation(const float* rot)
{
	Vector3 modelRot = mModel->GetRotation();
	if (modelRot != Vector3(rot[0], rot[1], rot[2]))
	{
		NotifyModelStateChangeListener();
		mModel->mTransform.SetRotationInDegree(rot[0], rot[1], rot[2]);
	}
}

void InteractiveMovableObject::SetModelScale(const float* scale)
{
	Vector3 modelScale = mModel->GetScale();

	if (modelScale != Vector3(scale[0], scale[1], scale[2]))
		NotifyModelStateChangeListener();

	mModel->mTransform.SetScale(scale);
}

json InteractiveMovableObject::SaveData()
{
	json data;
	data["Position"] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
	data["Scale"] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
	data["Rotation"] = { mModel->GetRotation().x,mModel->GetRotation().y,mModel->GetRotation().z };
	return data;
}

void InteractiveMovableObject::LoadSaveData(json data)
{
	//Init Model
	Vector3 pos = Vector3(data["Position"][0], data["Position"][1], data["Position"][2]);
	mModel->mTransform.SetPosition(pos);
	//SetModelPosition(pos);

	//Init Scale
	Vector3 scale = Vector3(data["Scale"][0], data["Scale"][1], data["Scale"][2]);
	SetModelScale(scale);

	Vector3 rotation = Vector3(data["Rotation"][0], data["Rotation"][1], data["Rotation"][2]);
	SetModelRotation(rotation);


	NotifyModelStateChangeListener();
}

void InteractiveMovableObject::InitCollider()
{
	if (!mModel)
	{
		DebugLog::LogError("{} Assimpモデルロード失敗", mObjectName);
		return;
	}

	Vector3 min = {
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max()
	};
	Vector3 max = {
		std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest()
	};


	for (const auto& vtx : mModel->mVertices)
	{
		min.x = std::min(min.x, vtx.pos.x);
		min.y = std::min(min.y, vtx.pos.y);
		min.z = std::min(min.z, vtx.pos.z);

		max.x = std::max(max.x, vtx.pos.x);
		max.y = std::max(max.y, vtx.pos.y);
		max.z = std::max(max.z, vtx.pos.z);

	}

	Vector3 center = {
		(min.x + max.x) / 2.0f,
		(min.y + max.y) / 2.0f,
		(min.z + max.z) / 2.0f,
	};

	Vector3 extents = {
		(max.x - min.x) / 2.0f,
		(max.y - min.y) / 2.0f,
		(max.z - min.z) / 2.0f,
	};

	


	mCollider = std::make_unique<BoxCollider>();
	mCollider->SetCenter(center);
	mCollider->SetExtents(extents);
	mColliderExtents = extents;

#ifdef _DEBUG
	mDebugColliderMesh = std::make_unique<Cube>();
	mDebugColliderMesh->Init(nullptr);
	mDebugColliderMesh->LoadDefShader();
	mDebugColliderMesh->SetDiffuse(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.4f));
#endif
}

void InteractiveMovableObject::TriggerListener()
{
}

void InteractiveMovableObject::PreUpdate(float dt)
{//もしモデルの位置や大きさが変化したら
	if (GetModelStateChange())
	{
		UpdateCollider();
		ClearModelStateChangeListener();
	}

	ObjectState prev = static_cast<ObjectState>(mObjectState);



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
			if (KInput::IsKeyPress(VK_LBUTTON))
			{
				mObjectState = static_cast<int>(ObjectState::STATE_DRAG);
			}
			else
			{
				mObjectState = static_cast<int>(ObjectState::STATE_HOVER);
			}
			
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
			if (KInput::IsKeyPress(VK_LBUTTON))
			{
				mObjectState = static_cast<int>(ObjectState::STATE_DRAG);
			}
			else
			{
				mObjectState = static_cast<int>(ObjectState::STATE_HOVER);
			}

		}
		else
		{
			mObjectState = static_cast<int>(ObjectState::STATE_NONE);
		}
		
	}
	break;
	case STATE_DRAG:
	{
		if (KInput::IsKeyPress(VK_LBUTTON))
		{
			mObjectState = static_cast<int>(ObjectState::STATE_DRAG);
		}
		else
		{
			mObjectState = static_cast<int>(ObjectState::STATE_RELEASE);
		}
	}
	break;
	case STATE_RELEASE:
		mObjectState = static_cast<int>(ObjectState::STATE_NONE);
		break;
	}


#ifdef _DEBUG
	//================================
	//	オブジェクトの位置などを調整する
	//================================
	if (ImGui::Begin(mObjectName.c_str()))
	{
		float scale[3] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
		ImGui::InputFloat3("Scale", scale);
		SetModelScale(scale);

		float pos[3] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
		ImGui::InputFloat3("Position", pos);
		SetModelPosition(pos);

		/*float rotation[3] = { mModel->GetRotation().x,mModel->GetRotation().y,mModel->GetRotation().z };
		ImGui::InputFloat3("Rotation", rotation);
		SetModelRotation(rotation);*/


		ImGui::Checkbox("ShowCollider", &isShowCollider);

	}
	ImGui::End();

	mDebugColliderMesh->SetScale(mCollider->GetScale());
	mDebugColliderMesh->SetPosition(mCollider->GetCenter());
#endif
}

void InteractiveMovableObject::GameUpdate(float dt)
{
	switch (mObjectState)
	{
	default:
	case ObjectState::STATE_NONE:
		OnStateNone(dt);
		break;
	case ObjectState::STATE_HOVER:
		OnStateHover(dt);
		break;
	case ObjectState::STATE_DRAG:
		OnStateDrag(dt);
		break;
	case ObjectState::STATE_RELEASE:
		OnStateReleased(dt);
	}
}

void InteractiveMovableObject::LateUpdate(float dt)
{
	mModel->Update(dt);
}

void InteractiveMovableObject::UpdateCollider()
{//Update Collider Scale
	const Vector3 Extents = mColliderExtents;
	Vector3 newScale = mModel->GetScale();
	newScale *= Extents;
	mCollider->SetExtents(newScale);

	//Update Rotation
	mCollider->SetOrientation(mModel->GetQuaternion());

	//Update Position
	Vector3 newCenter = mModel->GetPosition();
	newCenter.y += mCollider->GetExtents().y;
	mCollider->SetCenter(newCenter);

#ifdef _DEBUG
	mDebugColliderMesh->SetScale(newScale * 2.f);
	mDebugColliderMesh->SetPosition(newCenter);
	Vector3 rot = mModel->GetRotation();
	mDebugColliderMesh->mTransform.SetRotationInRadian(rot);
#endif

}

void InteractiveMovableObject::NotifyModelStateChangeListener()
{
	isModelStateChange = true;
}

void InteractiveMovableObject::ClearModelStateChangeListener()
{
	isModelStateChange = false;
}

void InteractiveMovableObject::SetDiffuse(DirectX::XMFLOAT4 color)
{
	mModel->SetDiffuse(color);
}

void InteractiveMovableObject::OnStateNone(float dt)
{
	mEffect.rimIntensity = 0.0f;
}

void InteractiveMovableObject::OnStateHover(float dt)
{
	// Set Rim Intensity
	if (isUseRimLight)mEffect.rimIntensity = 1.0f;

}

void InteractiveMovableObject::OnStateClicked(float dt)
{
	// Output Trigger
	isClicked = true;

	// Set Rim Intensity
	if (isUseRimLight)mEffect.rimIntensity = 1.0f;

}

void InteractiveMovableObject::OnStateDrag(float dt)
{
	isDragged = true;

}

void InteractiveMovableObject::OnStateReleased(float dt)
{
}

void InteractiveMovableObject::SetUseRimLight(bool isUseRimLight)
{
	this->isUseRimLight = isUseRimLight;
}

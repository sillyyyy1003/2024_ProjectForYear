#include "InteractiveStaticObject.h"
#include <memory>

#include "GampApp.h"
#include "GUI.h"
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

InteractiveStaticObject::InteractiveStaticObject()
{
}

void InteractiveStaticObject::Init(const char* filePath, const char* _objName)
{
	mPBRModel = std::make_shared<PBRModel>();
	mPBRModel->InitWithoutTex(filePath);
	mObjectName = _objName;
	InitCollider();
}


void InteractiveStaticObject::Init(std::shared_ptr<PBRModel> _model, const char* _objName)
{
	mPBRModel = _model;
	mObjectName = _objName;
	InitCollider();
}

void InteractiveStaticObject::LoadTex(PBRConfig::PBRTexList list)
{
	mPBRModel->LoadTex(list);
}

void InteractiveStaticObject::LoadShaderFile(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	mPBRModel->Primitive::LoadDefShader(vs,ps);
}

void InteractiveStaticObject::LoadShaderFile(const char* vsFile, const char* psFile)
{
	mPBRModel->Primitive::LoadDefShader(vsFile, psFile);
}


void InteractiveStaticObject::InitCollider()
{
	if (!mPBRModel)
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

	std::vector<std::vector<Vertex::VtxPosNormalTangentTex>> vtxGroups = mPBRModel->GetPBRVertices();

	for (const auto& vtxGroup : vtxGroups)
	{
		for (const auto& v : vtxGroup)
		{
			min.x = std::min(min.x, v.pos.x);
			min.y = std::min(min.y, v.pos.y);
			min.z = std::min(min.z, v.pos.z);

			max.x = std::max(max.x, v.pos.x);
			max.y = std::max(max.y, v.pos.y);
			max.z = std::max(max.z, v.pos.z);
		}
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

#ifdef _DEBUG
	mDebugColliderMesh = std::make_unique<Cube>();
	mDebugColliderMesh->Init(nullptr);
	mDebugColliderMesh->LoadDefShader();
	mDebugColliderMesh->SetDiffuse(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.1f));
#endif
}

void InteractiveStaticObject::Update(float dt)
{
	PreUpdate(dt);

	GameUpdate(dt);

	LateUpdate(dt);

}

void InteractiveStaticObject::Draw()
{
	mPBRModel->Draw();

#ifdef _DEBUG
	if(isShowCollider)
		mDebugColliderMesh->Draw();
#endif
}

void InteractiveStaticObject::SetModelPosition(DirectX::XMFLOAT3 pos)
{
	Vector3 modelPos = mPBRModel->GetPosition();
	if (modelPos != pos)
		NotifyModelStateChangeListener();
	mPBRModel->SetPosition(pos);
}

void InteractiveStaticObject::SetModelRotation(DirectX::XMFLOAT3 rot)
{
	Vector3 modelRot = mPBRModel->GetRotation();
	if (modelRot != rot)
	{
		NotifyModelStateChangeListener();
		mPBRModel->mTransform.SetRotationInDegree(rot);
	}
	
}

void InteractiveStaticObject::SetModelScale(DirectX::XMFLOAT3 scale)
{
	Vector3 modelScale = mPBRModel->GetScale();

	if (modelScale != scale)
		NotifyModelStateChangeListener();

	mPBRModel->SetScale(scale);

}

void InteractiveStaticObject::SetModelRotation(const float* rot)
{
	Vector3 modelRot = mPBRModel->GetRotation();
	if (modelRot != Vector3(rot[0],rot[1],rot[2]))
	{
		NotifyModelStateChangeListener();
		mPBRModel->mTransform.SetRotationInDegree(rot[0], rot[1], rot[2]);
	}

}

void InteractiveStaticObject::SetModelScale(const float* scale)
{
	Vector3 modelScale = mPBRModel->GetScale();

	if (modelScale != Vector3(scale[0],scale[1],scale[2]))
		NotifyModelStateChangeListener();

	mPBRModel->SetScale(scale);
}

json InteractiveStaticObject::SaveData()
{
	json data;
	data["Position"] = { mPBRModel->GetPosition().x,mPBRModel->GetPosition().y,mPBRModel->GetPosition().z };
	data["Scale"] = { mPBRModel->GetScale().x,mPBRModel->GetScale().y,mPBRModel->GetScale().z };

	return data;
}

void InteractiveStaticObject::LoadSaveData(json data, const char* objName)
{
	//Init Model
	//std::string filePath = data[objName]["Filepath"].get<std::string>();
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	mPBRModel->SetPosition(pos);

	//Init Scale
	Vector3 scale = Vector3(data[objName]["Scale"][0], data[objName]["Scale"][1], data[objName]["Scale"][2]);
	mPBRModel->SetScale(scale);

	mObjectName = objName;

	UpdateCollider();
}

void InteractiveStaticObject::SetModelPosition(const float* position)
{
	Vector3 modelPos = mPBRModel->GetPosition();
	if (modelPos != Vector3(position[0],position[1],position[2]))
		NotifyModelStateChangeListener();

	mPBRModel->SetPosition(position);
}

void InteractiveStaticObject::TriggerListener()
{
}

void InteractiveStaticObject::PreUpdate(float dt)
{
	//もしモデルの位置や大きさが変化したら
	if (GetModelStateChange())
	{
		UpdateCollider();
		ClearModelStateChangeListener();
	}

	ObjectState prev = static_cast<ObjectState>(mObjectState);

	

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
			}else
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
			}else
			{
				mObjectState = static_cast<int>(ObjectState::STATE_NONE);
			}
		}
		break;
	}


#ifdef _DEBUG
	//================================
	//	オブジェクトの位置などを調整する
	//================================
	if (ImGui::Begin(mObjectName.c_str()))
	{
		float scale[3] = { mPBRModel->GetScale().x,mPBRModel->GetScale().y,mPBRModel->GetScale().z };
		ImGui::InputFloat3("Scale", scale);
		SetModelScale(scale);

		float pos[3] = { mPBRModel->GetPosition().x,mPBRModel->GetPosition().y,mPBRModel->GetPosition().z };
		ImGui::InputFloat3("Position", pos);
		SetModelPosition(pos);

		/*float rotation[3] = { mPBRModel->GetRotation().x,mPBRModel->GetRotation().y,mPBRModel->GetRotation().z };
		ImGui::InputFloat3("Rotation", rotation);
		SetModelRotation(rotation);*/

		
		ImGui::Checkbox("Reset Material", &isShowCollider);

	}
	ImGui::End();

	mDebugColliderMesh->SetScale(mCollider->GetScale());
	mDebugColliderMesh->SetPosition(mCollider->GetCenter());
#endif
}

void InteractiveStaticObject::GameUpdate(float dt)
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

void InteractiveStaticObject::LateUpdate(float dt)
{
#ifdef _DEBUG
	mDebugColliderMesh->Update(dt);
#endif
	mPBRModel->Update(dt);

	mPBRModel->GetDefPS()->WriteShader(2, &mEffect);
}

void InteractiveStaticObject::UpdateCollider()
{
	//Update Collider Scale
	static const Vector3 Extents = mCollider->GetExtents();
	Vector3 newScale = mPBRModel->GetScale();
	newScale *= Extents;
	mCollider->SetExtents(newScale);

	//Update Rotation
	mCollider->SetOrientation(mPBRModel->GetQuaternion());

	//Update Position
	Vector3 newCenter = mPBRModel->GetPosition();
	newCenter.y += mCollider->GetExtents().y;
	mCollider->SetCenter(newCenter);
}

void InteractiveStaticObject::NotifyModelStateChangeListener()
{
	isModelStateChange = true;
}

void InteractiveStaticObject::ClearModelStateChangeListener()
{
	isModelStateChange = false;
}

void InteractiveStaticObject::OnStateNone()
{
	mEffect.rimIntensity = 0.0f;
}

void InteractiveStaticObject::OnStateHover()
{
	mEffect.rimIntensity = 2.0f;
}

void InteractiveStaticObject::OnStateClicked()
{
	// Output Trigger
	isClicked = true;

	// Set Rim Intensity
	mEffect.rimIntensity = 0.0f;
}

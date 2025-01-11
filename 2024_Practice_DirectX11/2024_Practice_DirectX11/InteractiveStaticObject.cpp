#include "InteractiveStaticObject.h"
#include <memory>

#include "Capsule.h"
#include "Circle.h"
#include "Cylinder.h"
#include "CylinderOneCap.h"
#include "GameApp.h"
#include "GUI.h"
#include "KInput.h"
#include "Model.h"
#include "Sphere.h"
#include "Square.h"

enum ObjectState 
{
	STATE_NONE = 0,
	STATE_HOVER = 1,
	STATE_CLICK = 2,
	STATE_MAX
};

using namespace DirectX;
using namespace DirectX::SimpleMath;

InteractiveStaticObject::InteractiveStaticObject()
{
}

void InteractiveStaticObject::InitPBRModel(const char* filePath, const char* _objName)
{
	mModel = std::make_shared<PBRModel>();
	mModel->InitWithoutTex(filePath);
	mObjectName = _objName;
	isUsePBRModel = true;
	InitCollider();
}


void InteractiveStaticObject::InitPBRModel(std::shared_ptr<PBRModel> _model, const char* _objName)
{
	mModel = _model;
	mObjectName = _objName;
	isUsePBRModel = true;
	InitCollider();
}

void InteractiveStaticObject::InitModel(const char* filePath, const char* _objName)
{
	mModel = std::make_unique<Model>();
	mModel->Init(filePath,{1,1});
	mObjectName = _objName;
	isUsePBRModel = false;
	InitCollider();
}

void InteractiveStaticObject::InitModel(const std::shared_ptr<Texture>& _file, const char* _objName)
{
}

void InteractiveStaticObject::Init(PrimitiveConfig::PrimitiveKind _kind, const std::shared_ptr<Texture>& texture,const char* _objName, DirectX::XMINT2 _UVSplit)
{
	
	switch (_kind)
	{
	default:
	case PrimitiveConfig::CAPSULE:
		mModel = std::make_unique<Capsule>();
		mModel->Init(texture, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CUBE:
		mModel = std::make_unique<Cube>();
		mModel->Init(texture, _UVSplit);
		break;
	case PrimitiveConfig::SPHERE:
		mModel = std::make_unique<Sphere>();
		mModel->Init(texture, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CYLINDER:
		mModel = std::make_unique<Cylinder>();
		mModel->Init(texture, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CYLINDER_ONECAP:
		mModel = std::make_unique<CylinderOneCap>();
		mModel->Init(texture, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::SQUARE:
		mModel = std::make_unique<Square>();
		mModel->Init(texture, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CIRCLE:
		mModel = std::make_unique<Circle>();
		mModel->Init(texture, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::MULTI:
		assert("The Model can't be created by texture!");
		break;
	}
	mObjectName = _objName;
	isUsePBRModel = false;
	InitCollider();
	
}

void InteractiveStaticObject::LoadTex(PBRConfig::PBRTexList list)
{
	mModel->LoadTex(list);
	
}

void InteractiveStaticObject::LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	mModel->Primitive::LoadDefShader(vs,ps);
}

void InteractiveStaticObject::LoadDefShader(const char* vsFile, const char* psFile)
{
	mModel->Primitive::LoadDefShader(vsFile, psFile);
}


void InteractiveStaticObject::UseRimLightEffect()
{
	isUseRimLight = true;
}

void InteractiveStaticObject::DisableRimLightEffect()
{
	isUseRimLight = false;
}

void InteractiveStaticObject::SetRimLightIntensity(float intensity)
{
	mEffect.rimIntensity = intensity;
}

void InteractiveStaticObject::SetRimLightColor(const DirectX::XMFLOAT4& color)
{
	mEffect.rimColor = { color.x,color.y,color.z };
}

void InteractiveStaticObject::InitCollider()
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

	if (extents.y == 0)
		extents.y = 0.1f;

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

void InteractiveStaticObject::Update(float dt)
{
	PreUpdate(dt);

	GameUpdate(dt);

	LateUpdate(dt);

}

void InteractiveStaticObject::Draw()
{
	mModel->GetDefPS()->WriteShader(2, &mEffect);
	mModel->Draw();

#ifdef _DEBUG
	if(isShowCollider)
		mDebugColliderMesh->Draw();
#endif
}

void InteractiveStaticObject::DrawWithPosition(DirectX::XMFLOAT3 pos)
{
}

bool InteractiveStaticObject::GetClicked()
{
	if (isClicked)
	{
		isClicked = false;
		return true;
	}else
	{
		return false;
	}
}

void InteractiveStaticObject::SetModelPosition(DirectX::XMFLOAT3 pos)
{
	Vector3 modelPos = mModel->GetPosition();
	if (modelPos != pos)
		NotifyModelStateChangeListener();
	mModel->mTransform.SetPosition(pos);
}

void InteractiveStaticObject::SetModelRotation(DirectX::XMFLOAT3 rot)
{
	Vector3 modelRot = mModel->GetRotation();
	if (modelRot != rot)
	{
		NotifyModelStateChangeListener();
		mModel->mTransform.SetRotationInRadian(rot);
	}
	
}

void InteractiveStaticObject::SetModelScale(DirectX::XMFLOAT3 scale)
{
	Vector3 modelScale = mModel->GetScale();

	if (modelScale != scale)
		NotifyModelStateChangeListener();

	mModel->mTransform.SetScale(scale);

}

void InteractiveStaticObject::SetModelRotation(const float* rot)
{
	Vector3 modelRot = mModel->GetRotation();
	if (modelRot != Vector3(rot[0],rot[1],rot[2]))
	{
		NotifyModelStateChangeListener();
		mModel->mTransform.SetRotationInDegree(rot[0], rot[1], rot[2]);
	}

}

void InteractiveStaticObject::SetModelScale(const float* scale)
{
	Vector3 modelScale = mModel->GetScale();

	if (modelScale != Vector3(scale[0],scale[1],scale[2]))
		NotifyModelStateChangeListener();

	mModel->mTransform.SetScale(scale);
}

json InteractiveStaticObject::SaveData()
{
	json data;
	data["Position"] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
	data["Scale"] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
	data["Rotation"] = { mModel->GetRotation().x,mModel->GetRotation().y,mModel->GetRotation().z };
	return data;
}

void InteractiveStaticObject::LoadSaveData(json data, const char* objName)
{
	//Init Model
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	SetModelPosition(pos);

	//Init Scale
	Vector3 scale = Vector3(data[objName]["Scale"][0], data[objName]["Scale"][1], data[objName]["Scale"][2]);
	SetModelScale(scale);

	Vector3 rotation = Vector3(data[objName]["Rotation"][0], data[objName]["Rotation"][1], data[objName]["Rotation"][2]);
	SetModelRotation(rotation);

	mObjectName = objName;

	NotifyModelStateChangeListener();
}

void InteractiveStaticObject::LoadSaveData(json data)
{
	//Init Model
	Vector3 pos = Vector3(data[mObjectName.c_str()]["Position"][0], data[mObjectName.c_str()]["Position"][1], data[mObjectName.c_str()]["Position"][2]);
	SetModelPosition(pos);

	//Init Scale
	Vector3 scale = Vector3(data[mObjectName.c_str()]["Scale"][0], data[mObjectName.c_str()]["Scale"][1], data[mObjectName.c_str()]["Scale"][2]);
	SetModelScale(scale);

	Vector3 rotation = Vector3(data[mObjectName.c_str()]["Rotation"][0], data[mObjectName.c_str()]["Rotation"][1], data[mObjectName.c_str()]["Rotation"][2]);
	SetModelRotation(rotation);

	NotifyModelStateChangeListener();
}

void InteractiveStaticObject::SetModelPosition(const float* position)
{
	Vector3 modelPos = mModel->GetPosition();
	if (modelPos != Vector3(position[0],position[1],position[2]))
		NotifyModelStateChangeListener();

	mModel->SetPosition(position);
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
		float scale[3] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
		ImGui::InputFloat3("Scale", scale);
		SetModelScale(scale);

		float pos[3] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
		ImGui::InputFloat3("Position", pos);
		SetModelPosition(pos);

		float rotation[3] = { mModel->GetRotation().x,mModel->GetRotation().y,mModel->GetRotation().z };
		ImGui::InputFloat3("Rotation", rotation);
		SetModelRotation(rotation);
		
		ImGui::Checkbox("ShowCollider", &isShowCollider);

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
	mModel->Update(dt);

}

void InteractiveStaticObject::UpdateCollider()
{
	//Update Collider Scale
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
	if (isUseRimLight)mEffect.rimIntensity = 0.0f;
}

void InteractiveStaticObject::OnStateHover()
{
	if (isUseRimLight)mEffect.rimIntensity = 1.0f;
}

void InteractiveStaticObject::OnStateClicked()
{
	// Output Trigger
	isClicked = true;

	// Set Rim Intensity
	if(isUseRimLight)mEffect.rimIntensity = 1.0f;
	
}

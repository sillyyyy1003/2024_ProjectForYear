#include "Ingredient.h"
#include "GameApp.h"
#include "GUI.h"
#include "KInput.h"
#include "Water.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

enum class ObjectState
{
	STATE_NONE,
	STATE_DRAG,
	STATE_SELECTED,
	
};


Ingredient::Ingredient()
{
}

void Ingredient::Init(const char* filePath, const char* _objName, PrimitiveConfig::PrimitiveKind _kind,
	DirectX::XMINT2 _UVSplit)
{
	if (_kind == PrimitiveConfig::PBR)
	{
		InitPBR(filePath, _objName);
		mModel->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_PBRModel"), SceneManager::Get()->GetObj<PixelShader>("PS_InterActiveObjectPBRModel"));
		InitCollider();
		return;
	}

	InitModel(filePath, _objName, _kind, _UVSplit);
	mModel->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_Primitives"), SceneManager::Get()->GetObj<PixelShader>("PS_InteractiveObjectNormal"));
	InitCollider();
}

void Ingredient::SetPrice(float _price)
{
	mFreshness = _price;
}


void Ingredient::SetName(const char* _ingredientName)
{
	mIngredientName = _ingredientName;
	mObjectName = mIngredientName;
}

void Ingredient::Update(float dt)
{
	if (isUsed)return;

	PreUpdate(dt);

	GameUpdate(dt);

	LateUpdate(dt);

	//StaticObject::Update(dt);
}

void Ingredient::Draw()
{
	if (isUsed)return;
	mModel->GetDefPS()->WriteShader(2, &mEffect);

	StaticObject::Draw();
#ifdef _DEBUG
	if (isShowCollider)
		mDebugColliderMesh->Draw();
#endif
}

void Ingredient::UpdatePrice()
{
	//todo: need to fix the transform problem
	
}


void Ingredient::SetPosition(DirectX::XMFLOAT3 pos)
{
	mModel->SetPosition(pos);
	isStateChange = true;
}

void Ingredient::SetScale(DirectX::XMFLOAT3 scale)
{
	mModel->SetScale(scale);
	isStateChange = true;
}

void Ingredient::InitCollider()
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

void Ingredient::PreUpdate(float dt)
{
	//Physical
	if (isStateChange)
	{
		UpdateCollider();
		isStateChange = false;
	}

	// Input
	ObjectState prev = static_cast<ObjectState>(mState);

	switch (prev)
	{
	default:

	case ObjectState::STATE_NONE:

		if (KInput::IsKeyTrigger(VK_LBUTTON))
		{
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
				mState =static_cast<int>(ObjectState::STATE_SELECTED);
			}
		}
		break;

	case ObjectState::STATE_SELECTED:

		if (KInput::IsKeyPress(VK_LBUTTON))
		{
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
				if (mousePos.x != mOldPos.x || mousePos.y != mOldPos.y)
				{
					mState =(int) ObjectState::STATE_DRAG;
					isStateChange = true;
				}
			}
			else
			{
				mState =(int) ObjectState::STATE_NONE;
				isStateChange = false;
			}
		}

		break;
	case ObjectState::STATE_DRAG:

		if (KInput::IsKeyPress(VK_LBUTTON))
		{
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
				isStateChange = true;
			}
			else
			{
				mState = static_cast<int>(ObjectState::STATE_NONE);
			}
		}
		else
		{
			mState = static_cast<int>(ObjectState::STATE_NONE);
		}
		break;

	}

	if (prev != static_cast<ObjectState>(mState))
	{
		GetCursorPos(&mOldPos);
	}

#ifdef _DEBUG
	//================================
	//	オブジェクトの位置などを調整する
	//================================
	if (ImGui::Begin(mObjectName.c_str()))
	{
		float pos[3] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
		ImGui::InputFloat3("Position", pos);
		SetPosition({pos[0],pos[1],pos[2]});

		float scale[3] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
		ImGui::InputFloat3("Scale", scale);
		SetScale({ scale[0],scale[1],scale[2]});

		/*float rotation[3] = { mModel->GetRotation().x,mModel->GetRotation().y,mModel->GetRotation().z };
		ImGui::InputFloat3("Rotation", rotation);
		SetModelRotation(rotation);*/

		ImGui::Checkbox("ShowCollider", &isShowCollider);

		GUI::ShowFloat3(mDebugColliderMesh->GetPosition());
		GUI::ShowFloat3(mDebugColliderMesh->GetScale());


	}
	ImGui::End();

	mDebugColliderMesh->SetScale(mCollider->GetScale());
	mDebugColliderMesh->SetPosition(mCollider->GetCenter());
#endif
}

void Ingredient::GameUpdate(float dt)
{
	
	switch (static_cast<ObjectState>(mState))
	{
	default:
	case ObjectState::STATE_NONE:
		OnStateNone(dt);
		break;
	case ObjectState::STATE_DRAG:
		OnStateDrag(dt);
		break;
	case ObjectState::STATE_SELECTED:
		OnStateSelected(dt);
		break;
	}
}

void Ingredient::LateUpdate(float dt)
{

}

void Ingredient::OnStateNone(float dt)
{
	mEffect.rimIntensity = 0.0f;

	//如果这个的x轴和z轴的位置在圆的范围内，则视为在水里
	Vector2 objPos = { mModel->GetPosition().x, mModel->GetPosition().z };
	Vector2 waterPos = { SceneManager::Get()->GetObj<Water>("PotionSceneWater")->GetPosition().x,SceneManager::Get()->GetObj<Water>("PotionSceneWater")->GetPosition().z };

	Vector2 distanceVector = objPos - waterPos;
	if (distanceVector.Length() < SceneManager::Get()->GetObj<Water>("PotionSceneWater")->GetRadius())
	{
		SceneManager::Get()->GetObj<Water>("PotionSceneWater")->UpdatColor(this->GetColor());
		isUsed = true;
	}



}

void Ingredient::OnStateDrag(float dt)
{
	GetCursorPos(&mOldPos);
	CameraBase* camera = GameApp::GetCurrentCamera();
	//カメラからマウス位置の方向ベクトルを取得
	XMVECTOR rayDir = camera->ScreenPointToRay(mOldPos);
	XMFLOAT3 rayDirection;
	XMStoreFloat3(&rayDirection, rayDir);

	//射線がxz平面上の位置を求める
	float t = -camera->GetPos().y / rayDirection.y;  // 计算 t 值
	Vector3 intersection = camera->GetPos() + t * rayDirection;  // 计算交点

	// intersection.x 和 intersection.z 就是射线在 XZ 平面上的坐标
	float xOnXYPlane = intersection.x;
	float zOnXYPlane = intersection.z;

	mModel->SetPosition(xOnXYPlane, mModel->mTransform.GetPosition().y, zOnXYPlane);

	//色変化
	// Set Rim Intensity
	mEffect.rimIntensity = 1.0f;
}

void Ingredient::OnStateSelected(float dt)
{
	// Set Rim Intensity
	mEffect.rimIntensity = 1.0f;
}

void Ingredient::UpdateCollider()
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
	mDebugColliderMesh->SetScale(newScale * 2.3f);
	mDebugColliderMesh->SetPosition(newCenter);
	Vector3 rot = mModel->GetRotation();
	mDebugColliderMesh->mTransform.SetRotationInRadian(rot);
#endif

}

std::string Ingredient::GetIngredientInfo()
{
	std::stringstream infoStr;

	infoStr << "Name: " << mIngredientName << "\n"
		<< "Freshness: " << mFreshness << "\n"
		<< "Price: " << mCurrPrice << "\n";



	return infoStr.str();
}



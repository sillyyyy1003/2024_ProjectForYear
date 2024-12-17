#include "Ingredient.h"

#include "GameApp.h"
#include "IngredientManager.h"
#include "InteractiveStaticObject.h"
#include "Potion.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
Ingredient::Ingredient()
{
}

void Ingredient::OnStateDrag(float dt)
{
	InteractiveMovableObject::OnStateDrag(dt);

	//始终把物体的z轴固定在0，在xy轴上移动物体
	GetCursorPos(&mOldPos);
	CameraBase* camera = GameApp::GetCurrentCamera();
	//カメラからマウス位置の方向ベクトルを取得
	XMVECTOR rayDir = camera->ScreenPointToRay(mOldPos);
	XMFLOAT3 rayDirection;
	XMStoreFloat3(&rayDirection, rayDir);

	//射線がxy平面上の位置を求める
	float t = -camera->GetPos().z / rayDirection.z;  // 计算 t 值
	Vector3 intersection = camera->GetPos() + t * rayDirection;  // 计算交点

	// intersection.x 和 intersection.y 就是射线在 XY 平面上的坐标
	float xOnXYPlane = intersection.x;
	float yOnXYPlane = intersection.y;

	SetModelPosition({ xOnXYPlane, yOnXYPlane, 0.0f });

	//if cursor isCollide the Pot;
	//カメラの位置を取得
	XMFLOAT3 camPos = camera->GetPos();
	XMVECTOR startPos = XMLoadFloat3(&camPos);
	float distance = 0;
	if(SceneManager::Get()->GetObj<InteractiveStaticObject>("pot")->GetCollider()->Interacts(startPos,rayDir,distance))
	{
		MixColorFunction(dt);
	}

	//Insert the object to controllable object
	IngredientManager::Get()->SetControllableIngredient(this);
}

void Ingredient::OnStateReleased(float dt)
{
	ResetPos();
	SetModelRotation({0, 0, 0});

	IngredientManager::Get()->ClearCurrentIngredient();
	isDragged = false;
	mAccumulateTime = 0.0f;//Reset Time;
}

void Ingredient::Draw()
{
	mModel->GetDefPS()->WriteShader(3, &mContainerData);
	InteractiveMovableObject::Draw();
}

void Ingredient::ResetPos()
{
	SetModelPosition(mDefaultPos);
}

void Ingredient::SetDefaultPos(DirectX::XMFLOAT3 pos)
{
	mDefaultPos = pos;
}

const DirectX::XMFLOAT4& Ingredient::GetColor()
{
	return mPigmentColor;
}

void Ingredient::SetPigmentColor(DirectX::XMFLOAT4 color)
{
	mPigmentColor = color;
	mContainerData.pigmentColor = { color.x,color.y,color.z };
	mModel->SetDiffuse({1.f,1.f,1.f,1.0f});
}

void Ingredient::AddColor(float dt)
{
	mAccumulateTime += dt;
	mCapacity -= (dt * 100.0f / 5.f);
	SceneManager::Get()->GetObj<Potion>("PotionSceneWater")->MixColor(this->GetColor(), mAlpha * mAccumulateTime * 0.05f);
}

void Ingredient::UpdateContainerColor()
{
	
	//求百分比
	float ratio = mCapacity / 100.f;
	float height = mCollider->GetExtents().y * 2 * ratio * cos(GetRotation().z);

	//求底部原点
	float bottomPos = mModel->GetPosition().y;
	mContainerData.capacityHeight = bottomPos + height;


}

void Ingredient::Update(float dt)
{
	if (!isControllable) return;
	InteractiveMovableObject::Update(dt);
	UpdateContainerColor();
}

json Ingredient::SaveData()
{
	json data;
	data["Graphic"]= InteractiveMovableObject::SaveData();
	data["DefaultPos"] = { mDefaultPos.x,mDefaultPos.y,mDefaultPos.z };
	data["PigmentColor"] = { mPigmentColor.x,mPigmentColor.y,mPigmentColor.z,mPigmentColor.w };
	return data;
}


void Ingredient::LoadSaveData(json data)
{

	InteractiveMovableObject::LoadSaveData(data[mObjectName.c_str()]["Graphic"]);

	//Set default Pos
	mDefaultPos = { data[mObjectName.c_str()]["DefaultPos"][0],data[mObjectName.c_str()]["DefaultPos"][1],data[mObjectName.c_str()]["DefaultPos"][2] };
	SetModelPosition(mDefaultPos);
	//Set pigment Color
	mPigmentColor = { data[mObjectName.c_str()]["PigmentColor"][0],data[mObjectName.c_str()]["PigmentColor"][1] ,data[mObjectName.c_str()]["PigmentColor"][2] ,data[mObjectName.c_str()]["PigmentColor"][3]};
	SetPigmentColor(mPigmentColor);

}

void Ingredient::SetIngredientAlpha(float alpha)
{
	mAlpha = alpha;
}

void Ingredient::ChargeCapacity()
{
	mCapacity = 90.f;
}

void Ingredient::SetCapacity(float _capacity)
{
	mCapacity = _capacity;
}

void Ingredient::MixColorFunction(float dt)
{
	if (mCapacity > 1)
	{
		AddColor(dt);
		SetRotation({ 0,0,60 });
		return;
	}

}

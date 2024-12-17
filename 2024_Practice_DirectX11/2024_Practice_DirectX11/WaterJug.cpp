#include "WaterJug.h"
#include "GameApp.h"
#include "IngredientManager.h"
#include "InteractiveStaticObject.h"
#include "Potion.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

WaterJug::WaterJug()
{
}

void WaterJug::OnStateDrag(float dt)
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
	if (SceneManager::Get()->GetObj<InteractiveStaticObject>("pot")->GetCollider()->Interacts(startPos, rayDir, distance))
	{
		AddWater(dt);
	}
	//Insert the object to controllable object
	IngredientManager::Get()->LockAllIngredient();
}

void WaterJug::OnStateReleased(float dt)
{
	ResetPos();
	//Reset rotation
	SetModelRotation({ 0, 0, 0 });
	isDragged = false;

	//Free all the ingredient
	IngredientManager::Get()->ClearCurrentIngredient();

}

void WaterJug::ResetPos()
{
	SetModelPosition(mDefaultPos);
}

void WaterJug::SetDefaultPos(DirectX::XMFLOAT3 pos)
{
	mDefaultPos = pos;
}

void WaterJug::Update(float dt)
{
	if (IngredientManager::Get()->GetControllableIngredient())return;
	InteractiveMovableObject::Update(dt);
}

json WaterJug::SaveData()
{
	json data;
	data["Graphic"] = InteractiveMovableObject::SaveData();
	data["DefaultPos"] = { mDefaultPos.x,mDefaultPos.y,mDefaultPos.z };
	return data;
}

void WaterJug::LoadSaveData(json data)
{
	InteractiveMovableObject::LoadSaveData(data[mObjectName.c_str()]["Graphic"]);

	//Set default Pos
	mDefaultPos = { data[mObjectName.c_str()]["DefaultPos"][0],data[mObjectName.c_str()]["DefaultPos"][1],data[mObjectName.c_str()]["DefaultPos"][2] };
	SetModelPosition(mDefaultPos);
}

void WaterJug::AddWater(float dt)
{
	mAccumulateTime += dt;
	SetModelRotation({ 0,0,60 });
	Potion* potion = SceneManager::Get()->GetObj<Potion>("PotionSceneWater").get();
	potion->Dilute(mAccumulateTime);
}



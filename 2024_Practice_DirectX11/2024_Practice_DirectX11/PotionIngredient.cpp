#include "PotionIngredient.h"

#include "GameApp.h"
#include "Potion.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void PotionIngredient::ResetPos()
{
	mGraphic->SetPosition(mDefaultPos.x, mDefaultPos.y);
}

void PotionIngredient::SetDefaultPos(DirectX::XMFLOAT2 pos)
{
	mDefaultPos = pos;
}

const DirectX::XMFLOAT4& PotionIngredient::GetColor()
{
	return mIngredientColor;
}

void PotionIngredient::OnStateHover(float dt)
{
	UI_MoveObject::OnStateHover(dt);
}

void PotionIngredient::OnStateDrag(float dt)
{
	UI_MoveObject::OnStateDrag(dt);
	//Set BackColor move with the object
	mFrontColor->SetPosition(mGraphic->GetPosition().x, mGraphic->GetPosition().y);

	POINT mousePos;
	GetCursorPos(&mousePos);
	CameraBase* camera = GameApp::GetCurrentCamera();
	//カメラからマウス位置の方向ベクトルを取得
	XMVECTOR rayDir = camera->ScreenPointToRay(mousePos);
	// 光线方向矢量（转换为 Vector3）
	Vector3 rayDirection;
	XMStoreFloat3(&rayDirection, rayDir);

	//StartPos
	Vector3 startPos = GameApp::GetCurrentCamera()->GetPos();

	float t = (SceneManager::Get()->GetObj<Potion>("PotionSceneWater")->GetPosition().y - startPos.y) / rayDirection.y;

	Vector2 pointPos = { startPos.x + rayDirection.x * t,startPos.z + rayDirection.x * t };

	Vector2 waterPos = { SceneManager::Get()->GetObj<Potion>("PotionSceneWater")->GetPosition().x,SceneManager::Get()->GetObj<Potion>("PotionSceneWater")->GetPosition().z };
	Vector2 distanceVec = pointPos - waterPos;

	if (distanceVec.Length() < SceneManager::Get()->GetObj<Water>("PotionSceneWater")->GetRadius())
	{
		AddColor(dt);
	}
}

void PotionIngredient::OnStateRelease(float dt)
{
	//Reset accumulate Time
	mAccumulateTime = 0;
	ResetPos();
}

json PotionIngredient::SaveData()
{
	json data;
	data["Graphic"] = UI_MoveObject::SaveData();
	data["Alpha"] = { mAlpha };
	data["Pigment"] = { mIngredientColor.x,mIngredientColor.y,mIngredientColor.z,mIngredientColor.w };
	return data;
}

void PotionIngredient::AddColor(float dt)
{
	mAccumulateTime += dt;
	mCapacity -= (dt * 100.0f / 10.f);
	SceneManager::Get()->GetObj<Potion>("PotionSceneWater")->MixColor(this->GetColor(), mAlpha * mAccumulateTime * 0.01f);
	
}

void PotionIngredient::UpdateFrontColor()
{
	float scaleY = mGraphic->mUiSet.mBackGround->GetScale().y;
	float newScaleY = scaleY * mCapacity / 100;

	mFrontColor->SetScale(mGraphic->mUiSet.mBackGround->GetScale().x, newScaleY);
	mFrontColor->SetPosition(mGraphic->mUiSet.mBackGround->GetPosition().x, mGraphic->mUiSet.mBackGround->GetPosition().y - (scaleY - newScaleY)/2.f);
}

void PotionIngredient::Draw()
{
	UI_MoveObject::Draw();
	mFrontColor->Draw();
}


void PotionIngredient::Update(float dt)
{
	
	PreUpdate(dt);
	GameUpdate(dt);
	mGraphic->Update();
	UpdateFrontColor();
}

void PotionIngredient::PreUpdate(float dt)
{
	UI_MoveObject::PreUpdate(dt);
}

void PotionIngredient::GameUpdate(float dt)
{
	switch (mState)
	{
	default:
	case UI_MoveObjectConfig::UiMoveState::STATE_NONE:
		OnStateNone(dt);
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_DRAG:
		OnStateDrag(dt);
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_HOVER:
		OnStateHover(dt);
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_RELEASE:
		OnStateRelease(dt);
		break;
	}
}

void PotionIngredient::LoadSaveData(json saveData)
{
	//Load Graphic 
	UI_MoveObject::LoadSaveData(saveData);

	//SetAlpha
	mAlpha = { saveData["Alpha"][0]};

	//SetColor
	mIngredientColor = { saveData["Pigment"][0],saveData["Pigment"][1] ,saveData["Pigment"][2] ,saveData["Pigment"][3] };

	//SetDefaultPos
	mDefaultPos = { mGraphic->GetPosition().x,mGraphic->GetPosition().y };

	//Init backColor
	mFrontColor = std::make_unique<UI_Square>();
	mFrontColor->Init(nullptr);
	mFrontColor->LoadDefShader();
	mFrontColor->SetDiffuseColor(mIngredientColor);
	//set back size
	mFrontColor->SetScale(mGraphic->GetContainerSize());
	//Set back pos
	mFrontColor->SetPosition(mGraphic->GetPosition());
	float posZ=mFrontColor->GetPosition().z;
	posZ -= 0.1f;
	mFrontColor->SetPosZ(posZ);

}

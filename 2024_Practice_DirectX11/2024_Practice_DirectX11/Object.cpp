#include "Object.h"
#include "Cube.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "GUI.h"
#include "KInput.h"
#include "Sphere.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


enum ObjectState
{
	STATE_NONE,
	STATE_DRAG,
	STATE_SELECTED,
	STATE_ROTATE,
	//STATE_MOVE,
};

Object::Object()
{
}

Object::~Object()
{
}

void Object::Init(PrimitiveKind kind, const char* fileName)
{
	switch(kind)
	{
	case CUBE:
		mModel = std::make_unique<Cube>();
		mCollider = std::make_unique<BoxCollider>();
		break;
	case SPHERE:
		mModel = std::make_unique<Sphere>();
		mCollider = std::make_unique<SphereCollider>();
		break;
	case CYLINDER:
		
		break;
	case PLANE:
		
		break;
	case CAPSULE:
		
		break;
	default:;
	}

	//モデルの初期化
	mModel->Init(fileName);
	mModel->mTransform.Rotate({ 0,0,45.0f});
}

void Object::Update(float dt)
{
	//入力状態の更新
	PreUpdate(dt);

	//入力状態によるオブジェクトの更新
	GameUpdate(dt);

	//あたり判定が終わるあとの処理
	LateUpdate(dt);
}

void Object::Draw()
{
	mModel->Draw();
}

void Object::PreUpdate(float dt)
{
	int prev = static_cast<ObjectState>(mState);

	switch (prev)
	{
	default:
	case STATE_NONE:
		if (KInput::IsKeyTrigger(VK_LBUTTON))
		{

			FirstPersonCamera* camera = GameApp::GetComponent<FirstPersonCamera>("Camera");
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
				mState = STATE_SELECTED;
				isStateChange = true;
			}
		}
		break;

	case STATE_SELECTED:
		
		if (KInput::IsKeyPress(VK_LBUTTON))
		{
			FirstPersonCamera* camera = GameApp::GetComponent<FirstPersonCamera>("Camera");
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
					mState = STATE_DRAG;
					isStateChange = true;
				}
			}
		}
		break;
	case STATE_DRAG:

		if(KInput::IsKeyRelease(VK_LBUTTON))
		{
			//mState = STATE_NONE;
			mState = STATE_SELECTED;
			isStateChange = true;
		}
		break;

	}

	if (prev != mState)
	{
		GetCursorPos(&mOldPos);
	}
	

}


void Object::GameUpdate(float dt)
{

	if (ImGui::Begin("Object Info"))
	{
		ImGui::Text("Position");
		GUI::ShowFloat3(mModel->mTransform.GetPosition());

		ImGui::Text("Rotation");
		GUI::ShowFloat3(mModel->GetRotation());

		ImGui::Text("Orientation");
		GUI::ShowFloat4(mCollider->GetOrientation());

		ImGui::Text("Center");
		GUI::ShowFloat3(mCollider->GetCenter());
	}

	ImGui::End();
	if (mState == STATE_NONE) { return; }

	switch(mState)
	{
	default:
	case STATE_DRAG:
		OnStateDrag(dt);
		break;
	
	case STATE_SELECTED:
		OnStateSelected(dt);
		break;
	}


	mModel->Update(dt);


}

void Object::LateUpdate(float dt)
{
	//Lazy Update->状態変更のときだけ、ステータス判定用のデータを更新する
	if(isStateChange)
	{
		mCollider->Transform(mModel->GetPosition(), mModel->mTransform.GetRotationQuatXM(),mModel->GetScale());
		isStateChange = false;
	}
		
}

void Object::OnStateDrag(float dt)
{
	GetCursorPos(&mOldPos);
	FirstPersonCamera* camera = GameApp::GetComponent<FirstPersonCamera>("Camera");
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

	mModel->mTransform.SetPosition(xOnXYPlane, yOnXYPlane, mModel->mTransform.GetPosition().z);
	
}

void Object::OnStateSelected(float dt)
{
	float rotateSpeed = 30.0f;
	mModel->mTransform.Rotate({ 0, 30 * dt,0 });
}

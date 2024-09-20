#include "Object.h"
#include "Cube.h"
#include "CylinderOneCap.h"
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

void Object::Init(PrimitiveKind kind)
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
	case CAPSULE:
		
		break;
	case CYLINDER_ONECAP:
		mModel = std::make_unique<CylinderOneCap>();
		mCollider = std::make_unique<BoxCollider>();
	default:;
	}

	//モデルの初期化
	//mModel->mTransform.Rotate({ 0,0,45.0f});
}

void Object::Init(PrimitiveKind kind, const char* filePath)
{
	Init(kind);
	InitModel(filePath);
}

void Object::InitModel(const char* filePath)
{
	mModel->Init(filePath);
}

void Object::LoadSaveData(json data, const char* objName)
{
	//Init Model
	std::string filePath = data[objName]["Filepath"].get<std::string>();
	InitModel(filePath.c_str());

	//Init Pos
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	mModel->SetPosition(pos);

	//Init Rotation
	Vector3 rotation = Vector3(data[objName]["Rotation"][0], data[objName]["Rotation"][1], data[objName]["Rotation"][2]);
	mModel->mTransform.SetRotationInDegree(rotation);

	//Init Scale
	Vector3 scale = Vector3(data[objName]["Scale"][0], data[objName]["Scale"][1], data[objName]["Scale"][2]);
	mModel->SetScale(scale);

	//Init Material
	Material mat = {
		Color(data[objName]["Material"]["Ambient"][0],data[objName]["Material"]["Ambient"][1],data[objName]["Material"]["Ambient"][2],data[objName]["Material"]["Ambient"][3]),
		Color(data[objName]["Material"]["Diffuse"][0],data[objName]["Material"]["Diffuse"][1],data[objName]["Material"]["Diffuse"][2],data[objName]["Material"]["Diffuse"][3]),
		Color(data[objName]["Material"]["Specular"][0],data[objName]["Material"]["Specular"][1],data[objName]["Material"]["Specular"][2],data[objName]["Material"]["Specular"][3]),
		Color(data[objName]["Material"]["Emission"][0],data[objName]["Material"]["Emission"][1],data[objName]["Material"]["Emission"][2],data[objName]["Material"]["Emission"][3]),
	};
	SetMaterial(mat);

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

void Object::SetMaterial(const Material& mat)
{
	mModel->SetMaterial(mat);
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

#ifdef _DEBUG
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
#endif
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
		mCollider->Transform(mModel->GetPosition(), mModel->mTransform.GetQuaternionXM(),mModel->GetScale());
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

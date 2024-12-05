#include "Object.h"
#include "Capsule.h"
#include "Cube.h"
#include "Cylinder.h"
#include "CylinderOneCap.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "GUI.h"
#include "KInput.h"
#include "PointLight.h"
#include "Sphere.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


enum ObjectState
{
	STATE_NONE,
	STATE_DRAG,
	STATE_SELECTED,
	

};

Object::Object()
{
}

Object::~Object()
{
}

void Object::Init(PrimitiveConfig::PrimitiveKind kind)
{
	switch(kind)
	{
	default:
	case PrimitiveConfig::CUBE:
		mModel = std::make_unique<Cube>();
		mCollider = std::make_unique<BoxCollider>();
		break;
	case PrimitiveConfig::SPHERE:
		mModel = std::make_unique<Sphere>();
		mCollider = std::make_unique<SphereCollider>();
		break;
	case PrimitiveConfig::CYLINDER:
		mModel = std::make_unique<Cylinder>();
		mCollider = std::make_unique<BoxCollider>();
		break;
	case PrimitiveConfig::CAPSULE:
		mModel = std::make_unique<Capsule>();
		mCollider = std::make_unique<BoxCollider>();
		break;
	case PrimitiveConfig::CYLINDER_ONECAP:
		mModel = std::make_unique<CylinderOneCap>();
		mCollider = std::make_unique<BoxCollider>();
	}

}

void Object::Init(PrimitiveConfig::PrimitiveKind kind, const char* filePath)
{
	Init(kind);
	InitModel(filePath);
	
}

void Object::InitModel(const char* filePath)
{
	mModel->Init(filePath, { 1,1 });
	mModel->LoadDefShader(SceneManager::Get()->GetObj<VertexShader>("VS_Primitives"), SceneManager::Get()->GetObj<PixelShader>("PS_Primitives"));
}

void Object::LoadSaveData(json data, const char* objName)
{
	
	//Init Pos
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	mModel->SetPosition(pos);

	//Init Rotation
	Vector3 rotation = Vector3(data[objName]["Rotation"][0], data[objName]["Rotation"][1], data[objName]["Rotation"][2]);
	mModel->mTransform.SetRotationInDegree(rotation);

	//Init Scale
	Vector3 scale = Vector3(data[objName]["Scale"][0], data[objName]["Scale"][1], data[objName]["Scale"][2]);
	mModel->SetScale(scale);

	//Collider データ更新
	mCollider->Transform(mModel->GetPosition(), mModel->mTransform.GetQuaternionXM(), mModel->GetScale());
	

	//Init Material
	Material mat = {
		Color(data[objName]["Material"]["Ambient"][0],data[objName]["Material"]["Ambient"][1],data[objName]["Material"]["Ambient"][2],data[objName]["Material"]["Ambient"][3]),
		Color(data[objName]["Material"]["Diffuse"][0],data[objName]["Material"]["Diffuse"][1],data[objName]["Material"]["Diffuse"][2],data[objName]["Material"]["Diffuse"][3]),
		Color(data[objName]["Material"]["Specular"][0],data[objName]["Material"]["Specular"][1],data[objName]["Material"]["Specular"][2],data[objName]["Material"]["Specular"][3]),
		Color(data[objName]["Material"]["Emission"][0],data[objName]["Material"]["Emission"][1],data[objName]["Material"]["Emission"][2],data[objName]["Material"]["Emission"][3]),
	};
	SetMaterial(mat);

	//mObjectName = objName;
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

json Object::SaveData()
{
	json data;
	data["Position"] = {GetPosition().x,GetPosition().y,GetPosition().z };
	data["Scale"] = { GetScale().x,GetScale().y,GetScale().z };
	data["Rotation"] = { GetRotation().x,GetRotation().y,GetRotation().z };

	//Set Material
	data["Material"]["Ambient"] = { GetMaterial().ambient.x,GetMaterial().ambient.y, GetMaterial().ambient.z, GetMaterial().ambient.w };
	data["Material"]["Diffuse"] = { GetMaterial().diffuse.x, GetMaterial().diffuse.y, GetMaterial().diffuse.z, GetMaterial().diffuse.w };
	data["Material"]["Specular"] = { GetMaterial().specular.x,GetMaterial().specular.y,GetMaterial().specular.z,GetMaterial().specular.w };
	data["Material"]["Emission"] = { GetMaterial().emission.x, GetMaterial().emission.y, GetMaterial().emission.z, GetMaterial().emission.w };

	return data;
}



void Object::PreUpdate(float dt)
{
	//Physical
	if (isStateChange)
	{
		mCollider->Transform(mModel->GetPosition(), mModel->mTransform.GetQuaternionXM(), mModel->GetScale());
		isStateChange = false;
	}

	// Input
	int prev = static_cast<ObjectState>(mState);

	switch (prev)
	{
	default:
	case STATE_NONE:

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
				mState = STATE_SELECTED;
			}
		}
		break;

	case STATE_SELECTED:
	
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
					mState = STATE_DRAG;
					isStateChange = true;
				}
			}else
			{
				mState = STATE_NONE;
				isStateChange = false;
			}
		}

		break;
	case STATE_DRAG:

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
				mState = STATE_NONE;
			}
		}
		else
		{
			mState = STATE_NONE;
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

// オブジェクトの情報表示
#ifdef _DEBUG

	GUI::ObjectSetting(this, mObjectName.c_str());
	

#endif

	switch(mState)
	{
	default:
	case STATE_NONE:
		OnStateNone(dt);
		break;
	case STATE_DRAG:
		OnStateDrag(dt);
		break;
	case STATE_SELECTED:
		OnStateSelected(dt);
		break;
	}


}

void Object::LateUpdate(float dt)
{
	//Render
	mModel->Update(dt);

}

void Object::OnStateDrag(float dt)
{
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

	mModel->mTransform.SetPosition(xOnXYPlane, yOnXYPlane, mModel->mTransform.GetPosition().z);

	//色変化
	mEffect = { 1.0f,1.0f,1.0f ,1.0f };
}

void Object::OnStateSelected(float dt)
{
	//色変化
	mEffect = { 1.0f,1.0f,1.0f ,1.0f };
	
}

void Object::OnStateNone(float dt)
{
	mEffect = { 0.85f,0.85f ,0.85f ,1.0f };
}

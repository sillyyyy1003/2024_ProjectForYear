#include "FirstPersonCamera.h"

#include <complex>

#include "KInput.h"
using namespace DirectX;

enum CameraKind
{
    ///No Interaction
    CAM_NONE,
    ///MOVE FREE
    CAM_FREE,
};

void FirstPersonCamera::Update(float dt)
{
    UpdateState();
    if (mState == CAM_NONE) return;

    // マウス移動量
    POINT cursorPos;
    GetCursorPos(&cursorPos);
	XMFLOAT2 mouseMove = DirectX::XMFLOAT2((float)cursorPos.x - mOldPos.x, (float)cursorPos.y - mOldPos.y);
    mOldPos = cursorPos;


    switch (mState)
    {
    case CAM_FREE:	UpdateFlight(mouseMove, dt);	break;
    default:;
    }
}

void FirstPersonCamera::SetPosition(float x, float y, float z)
{
    CameraBase::SetPos(XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const XMFLOAT3& pos)
{
    CameraBase::SetPos(pos);
}

void FirstPersonCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
    mTransform.SetPosition(pos);
    mTransform.LookAt(target, up);
   
}

void FirstPersonCamera::LookTo(const XMFLOAT3& pos, const XMFLOAT3& to, const XMFLOAT3& up)
{
    mTransform.SetPosition(pos);
    mTransform.LookTo(to, up);

}

void FirstPersonCamera::Strafe(float d)
{
    mTransform.Translate(mTransform.GetRightAxis(), d);
}

void FirstPersonCamera::Walk(float d)
{
    XMFLOAT3 rightAxis = mTransform.GetRightAxis();
    XMVECTOR rightVec = XMLoadFloat3(&rightAxis);
    XMVECTOR frontVec = XMVector3Normalize(XMVector3Cross(rightVec, g_XMIdentityR1));
    XMFLOAT3 front;
    XMStoreFloat3(&front, frontVec);
    mTransform.Translate(front, d);
}

void FirstPersonCamera::MoveForward(float d)
{
    mTransform.Translate(mTransform.GetForwardAxis(), d);
}

void FirstPersonCamera::Pitch(float rad)
{
    XMFLOAT3 rotation = mTransform.GetRotation();
    // LIMIT ROTATE DEGREE TO ±70°
    rotation.x += rad;
    if (rotation.x > XM_PI * 7 / 18)
        rotation.x = XM_PI * 7 / 18;
    else if (rotation.x < -XM_PI * 7 / 18)
        rotation.x = -XM_PI * 7 / 18;

    mTransform.SetRotationInRadian(rotation);
}

void FirstPersonCamera::RotateY(float rad)
{
    XMFLOAT3 rotation = mTransform.GetRotation();
    rotation.y = XMScalarModAngle(rotation.y + rad);
    mTransform.SetRotationInRadian(rotation);
}

void FirstPersonCamera::MoveUpward(float d)
{
    mTransform.Translate(mTransform.GetUpAxis(), d);
}

void FirstPersonCamera::UpdateState()
{
    CameraKind prev = (CameraKind)mState;

    if (KInput::IsKeyPress(VK_MENU))//ALT
    {
        mState = CAM_NONE;
        if (KInput::IsKeyTrigger('R'))
            ResetCamera();//カメラの位置を元に戻す
    }
	else if(KInput::IsKeyPress(VK_RBUTTON))
    {
        mState = CAM_FREE;
    }
    else
    {
		mState = CAM_NONE;
    }

    if (prev != mState)
        GetCursorPos(&mOldPos);
}

void FirstPersonCamera::UpdateFlight(DirectX::XMFLOAT2 mouseMove, float dt)
{

	//横回転
    float angleX = 360.0f * mouseMove.x / WIN_WIDTH;
    RotateY(angleX * dt * mMoveSpeed);
    //縦回転
    float angleY = 180.0f * mouseMove.y / WIN_HEIGHT;
    Pitch(angleY * dt * mMoveSpeed);

	// キー入力で移動
    if (KInput::IsKeyPress('W'))//Move Forward
        Walk(dt * mMoveSpeed);
    if (KInput::IsKeyPress('S'))//Move Backward
        Walk(dt * -mMoveSpeed);
    if (KInput::IsKeyPress('A'))//Move Left
        Strafe(dt * -mMoveSpeed);
    if (KInput::IsKeyPress('D'))//Move Right
        Strafe(dt * mMoveSpeed);
    if (KInput::IsKeyPress('Q'))
        MoveUpward(dt * mMoveSpeed);
    if (KInput::IsKeyPress('E'))
        MoveUpward(dt * -mMoveSpeed);
}


#include "ThirdPersonCamera.h"

#include "KInput.h"
using namespace DirectX;

enum CameraKind
{
    ///No Interaction
    CAM_NONE,
    ///ZOOM IN/OUT
    CAM_DOLLY,
};

ThirdPersonCamera::~ThirdPersonCamera()
{

}

void ThirdPersonCamera::Update(float dt)
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
    case CAM_DOLLY:	UpdateDolly(mouseMove, dt);	break;
    default:;
    }
        

}

DirectX::XMFLOAT3 ThirdPersonCamera::GetTargetPosition() const
{
	return mTarget;
}

float ThirdPersonCamera::GetDistance() const
{
	return mDistance;
}

void ThirdPersonCamera::RotateX(float rad)
{
    XMFLOAT3 rotation = mTransform.GetRotation();
    // 将绕x轴旋转弧度限制在[0, pi/3]之间
    rotation.x += rad;
    if (rotation.x < 0.0f)
        rotation.x = 0.0f;
    else if (rotation.x > XM_PI / 3)
        rotation.x = XM_PI / 3;

    mTransform.SetRotationInRadian(rotation);
    mTransform.SetPosition(mTarget);
    mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::RotateY(float rad)
{
    XMFLOAT3 rotation = mTransform.GetRotation();
    rotation.y = XMScalarModAngle(rotation.y + rad);

    mTransform.SetRotationInRadian(rotation);
    mTransform.SetPosition(mTarget);
    mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::Approach(float dist)
{
    mDistance += dist;
    // Limit the distance
    if (mDistance < mMinDist)
        mDistance = mMinDist;
    else if (mDistance > mMaxDist)
        mDistance = mMaxDist;

    mTransform.SetPosition(mTarget);
    mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::SetRotationX(float rad)
{
    XMFLOAT3 rotation = mTransform.GetRotation();
    // Limit the angel to (0~pi/3)
    rotation.x = rad;
    if (rotation.x < 0.0f)
        rotation.x = 0.0f;
    else if (rotation.x > XM_PI / 3)
        rotation.x = XM_PI / 3;

    mTransform.SetRotationInRadian(rotation);
    mTransform.SetPosition(mTarget);
    mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::SetRotationY(float rad)
{
    XMFLOAT3 rotation = mTransform.GetRotation();
    rotation.y = XMScalarModAngle(rad);
    mTransform.SetRotationInRadian(rotation);
    mTransform.SetPosition(mTarget);
    mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::SetTarget(const DirectX::XMFLOAT3& target)
{
    mTarget = target;
}

void ThirdPersonCamera::SetDistance(float dist)
{
    mDistance = dist;
}

void ThirdPersonCamera::SetDistanceMinMax(float minDist, float maxDist)
{
    mMinDist = minDist;
    mMaxDist = maxDist;
}

void ThirdPersonCamera::UpdateState()
{
    CameraKind prev = (CameraKind)mState;

    if (KInput::IsKeyPress(VK_MENU))//ALT
    {
        mState = CAM_NONE;
        if (KInput::IsKeyTrigger('R'))
            ResetCamera();//カメラの位置を元に戻す
    }
    else if (KInput::IsKeyPress(VK_RBUTTON))
    {
        mState = CAM_DOLLY;
    }
    else
    {
        mState = CAM_NONE;
    }

    if (prev != mState)
        GetCursorPos(&mOldPos);
}

void ThirdPersonCamera::UpdateDolly(DirectX::XMFLOAT2 mouseMove, float dt)
{

    //横回転
    float angleX = 360.0f * mouseMove.x / WIN_WIDTH;
    RotateY(angleX * dt * mMoveSpeed);
    //縦回転
    float angleY = 180.0f * mouseMove.y / WIN_HEIGHT;
    RotateX(angleY * dt * mMoveSpeed);

    // キー入力で移動
    if (KInput::IsKeyPress('W'))//Move Forward
		Approach(dt * mMoveSpeed);
    if (KInput::IsKeyPress('S'))//Move Backward
        Approach(dt * -mMoveSpeed);

}



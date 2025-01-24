#include "MovableStaticObject.h"

void MovableStaticObject::Update(float dt)
{
	GameUpdate(dt);

	StaticObject::Update(dt);
}

void MovableStaticObject::GameUpdate(float dt)
{
	switch(mState)
	{
	default:
	case ObjectMovement::STATE_NONE:
		
		break;
	case ObjectMovement::STATE_PITCH:
		Pitch(dt);
		break;
	case ObjectMovement::STATE_SPIN_IN:
		SpinIn(dt);
		break;
	case ObjectMovement::STATE_WALK:
		Walk(dt);
		break;
	}
}

void MovableStaticObject::SetSpinIn(float spinDegree, const DirectX::XMFLOAT3& spinCenter, const DirectX::XMFLOAT3& startPos, const DirectX::XMFLOAT3&  rotation, const DirectX::XMFLOAT3& startRotation, float duration, ObjectMovement::Axis axis, EaseOut::EasingMode mode)
{
	//移動時間
	mDuration = duration;
	//回転運動量設定
	mSpinDegree = spinDegree;
	//回転中心
	mSpinCenter = spinCenter;
	//回転軸
	mAxis = axis;
	//回転量
	mRotateAmount = rotation;
	//初期位置を設定する
	mStartPos = startPos;
	//初期回転を設定する
	mStartRotation = startRotation;

	//Easing 設定
	mEaseMode = mode;

	//次のステートを設定する
	mNextState = ObjectMovement::STATE_SPIN_IN;

}

void MovableStaticObject::SetWalk(const DirectX::XMFLOAT3& distance, float duration, const DirectX::XMFLOAT3& startPos,
	EaseOut::EasingMode mode, ObjectMovement::Axis axis)
{
	//移動量
	mDistance = distance;
	//移動時間
	mDuration = duration;
	//初期位置を設定する
	mStartPos = startPos;
	//Easing 設定
	mEaseMode = mode;
	//どの軸にイージングを適応する
	mAxis = axis;

	//次のステートを設定する
	mNextState = ObjectMovement::STATE_WALK;

}

void MovableStaticObject::SetMoveState(ObjectMovement::Movement state)
{
	mState = state;

	switch(state)
	{
	default:
	case ObjectMovement::STATE_NONE:
		break;
	case ObjectMovement::STATE_SPIN_IN:
		SetPosition(mStartPos);
		SetRotation(mStartRotation);
		break;
	case ObjectMovement::STATE_WALK:
		SetPosition(mStartPos);
		break;
	}
	

}

void MovableStaticObject::SetEasingMode(EaseOut::EasingMode mode)
{
	mEaseMode = mode;
}

float MovableStaticObject::Ease(float data)
{
	switch (mEaseMode)
	{
	case EaseOut::MODE_NONE:
		return data;
		break;
	case EaseOut::MODE_SINE:
		return EaseOut::EaseOutSine(data);
		break;
	case EaseOut::MODE_BACK:
		return EaseOut::EaseOutBack(data);
		break;
	case EaseOut::MODE_BOUNCE:
		return EaseOut::EaseOutBounce(data);
		break;
	case EaseOut::MODE_CIRC:
		return EaseOut::EaseOutCirc(data);
		break;
		default:
			return data;
	}
}


void MovableStaticObject::SpinIn(float dt)
{
	if (mAccumulateTime <= mDuration)
	{
		float easeStart = Ease(mAccumulateTime / mDuration);
		mAccumulateTime += dt;
		float easeEnd = Ease(mAccumulateTime / mDuration);
		float easeStep = easeEnd - easeStart;

		//回転運動
		float degree = mSpinDegree * easeStep;

		switch (mAxis)
		{
		default:
		case ObjectMovement::AxisX:
			this->mModel->mTransform.MoveAround(mSpinCenter, { 1,0,0 }, degree);
		break;
		case ObjectMovement::AxisY:
			this->mModel->mTransform.MoveAround(mSpinCenter, { 0,1,0 }, degree);
		break;
		case ObjectMovement::AxisZ:
			this->mModel->mTransform.MoveAround(mSpinCenter, { 0,0,1 }, degree);
		break;
		}

		//自身回転
		float rotateStep = dt / mDuration;
		Vector3 rotation = rotateStep * mRotateAmount;
		this->mModel->mTransform.Rotate(rotation);
		
	}else
	{
		//reset data;
		mAccumulateTime = 0.0f;
		mState = ObjectMovement::STATE_NONE;
		mNextState = ObjectMovement::STATE_NONE;
		isFinishMovement = true;
	}

}

void MovableStaticObject::Pitch(float dt)
{
}

void MovableStaticObject::Walk(float dt)
{
	if (mAccumulateTime < mDuration)
	{
		//EaseStepを求める
		float easeStart = Ease(mAccumulateTime / mDuration);
		mAccumulateTime += dt;
		float easeEnd = Ease(mAccumulateTime / mDuration);
		float easeStep = easeEnd - easeStart;

		//適応する軸によってEasing効果を実現する
		switch(mAxis)
		{
		case ObjectMovement::AxisX:
		{
			Vector3 pos = (dt / mDuration) * mDistance;
			pos.x = mDistance.x * easeStep;
			pos += GetPosition();
			SetPosition(pos);
		}
			break;
		case ObjectMovement::AxisY:
		{
			Vector3 pos = (dt / mDuration) * mDistance;
			pos.y = mDistance.y * easeStep;
			pos += GetPosition();
			SetPosition(pos);
		}
			break;
		case ObjectMovement::AxisZ:
		{
			Vector3 pos = (dt / mDuration) * mDistance;
			pos.z = mDistance.z * easeStep;
			pos += GetPosition();
			SetPosition(pos);
		}
			break;
		default:
			return;
		}

	}else
	{
		//リセット
		mAccumulateTime = 0;
		mState = ObjectMovement::STATE_NONE;
		mNextState = ObjectMovement::STATE_NONE;
		isFinishMovement = true;
	}
}



ObjectMovement::Movement MovableStaticObject::GetNextState()
{
	return static_cast<ObjectMovement::Movement>(mNextState);
}

bool MovableStaticObject::GetMovementFinished()
{
	if(isFinishMovement)
	{
		//Reset state
		isFinishMovement = false;
		return true;
	}else
	{
		return false;
	}
}


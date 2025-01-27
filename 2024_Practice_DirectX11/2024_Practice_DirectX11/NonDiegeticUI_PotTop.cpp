#include "NonDiegeticUI_PotTop.h"
#include "KInput.h"
#include "ResultManager.h"
#include "ScreenFadeEffect.h"

enum ObjectState
{
	STATE_NONE = 0,
	STATE_HOVER = 1,
	STATE_CLICK = 2,
	STATE_RESULT= 3 ,	//Š®¬‰‰o
	STATE_MAX 
};

enum ResultState
{
	STATE_RISE,
	STATE_SPIN,
	STATE_FALL,
	STATE_SHAKING,
};

using namespace DirectX;
using namespace DirectX::SimpleMath;

NonDiegeticUI_PotTop::NonDiegeticUI_PotTop()
{
}

void NonDiegeticUI_PotTop::PreUpdate(float dt)
{
#ifdef _DEBUG
	if(KInput::IsKeyTrigger(VK_SPACE))
	{
		mObjectState = STATE_NONE;
		ResetPosition();
	}

	if(ImGui::Begin("PoptopMovement"))
	{
		ImGui::InputFloat("duration", &mDuration);

		float distance[3] = { mDistance.x,mDistance.y,mDistance.z };
		ImGui::InputFloat3("Distance", distance);
		mDistance = Vector3(distance);

	}
	ImGui::End();

#endif

	if (mObjectState == STATE_RESULT)return;
	InteractiveStaticObject::PreUpdate(dt);
}

void NonDiegeticUI_PotTop::GameUpdate(float dt)
{
	switch (mObjectState)
	{
	case ObjectState::STATE_NONE:
		OnStateNone();
		break;
	case ObjectState::STATE_HOVER:
		OnStateHover();
		break;
	case ObjectState::STATE_CLICK:
		OnStateClicked();
		break;
	case ObjectState::STATE_RESULT:
		OnStateResult(dt);
		break;
	default:
		return;
	}
}

void NonDiegeticUI_PotTop::LoadSaveData(json data)
{
	InteractiveStaticObject::LoadSaveData(data);
	mDefaultPosition = GetPosition();
	mDefaultRotation = GetRotation();
}

void NonDiegeticUI_PotTop::OnStateClicked()
{
	//InteractiveStaticObject::OnStateClicked();
	mObjectState = STATE_RESULT;
	isFinishPotion = true;

	//ˆÚ“®‹——£‚ğİ’è
	mDistance = { 0.f,2.6f,0.0f };
	//ˆÚ“®ŠÔİ’è
	mDuration = 0.25f;
	//Ÿ‚Ìó‘Ô
	mResultState = STATE_RISE;
	//State after next
	mNextResultState = STATE_SPIN;
	//‰ñ“]Šp“x
	mRotateAmount = { 0,0,30.f };


}

void NonDiegeticUI_PotTop::OnStateHover()
{
	InteractiveStaticObject::OnStateHover();
}

void NonDiegeticUI_PotTop::OnStateNone()
{
	InteractiveStaticObject::OnStateNone();

}

void NonDiegeticUI_PotTop::OnStateResult(float dt)
{
	switch(mResultState)
	{
	case STATE_RISE:
		OnStateRise(dt);
		break;
	case STATE_SPIN:
		OnStateSpin(dt);
		break;
	case STATE_FALL:
		OnStateFall(dt);
		break;
	case STATE_SHAKING:
		OnStateShaking(dt);
		break;
	default:
		return;
	}
}

void NonDiegeticUI_PotTop::SetSpinIn(float spinDegree, const DirectX::XMFLOAT3& spinCenter,
	const DirectX::XMFLOAT3& startPos, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& startRotation,
	float duration, ObjectMovement::Axis axis)
{
	//ˆÚ“®ŠÔ
	mDuration = duration;
	//‰ñ“]‰^“®—Êİ’è
	mSpinDegree = spinDegree;
	//‰ñ“]’†S
	mSpinCenter = spinCenter;
	//‰ñ“]²
	mAxis = axis;
	//‰ñ“]—Ê
	mRotateAmount = rotation;
	//‰ŠúˆÊ’u‚ğİ’è‚·‚é
	mStartPos = startPos;
	//‰Šú‰ñ“]‚ğİ’è‚·‚é
	mStartRotation = startRotation;
}

void NonDiegeticUI_PotTop::OnStateRise(float dt)
{
	if (mAccumulateTime <= mDuration)
	{
		float easeStart = EaseOut::EaseOutCubic(mAccumulateTime / mDuration);
		mAccumulateTime += dt;
		float easeEnd = EaseOut::EaseOutCubic(mAccumulateTime / mDuration);
		float easeStep = easeEnd - easeStart;

		Vector3 pos = mDistance * easeStep;
		pos += GetPosition();
		SetModelPosition(pos);

		float rotateStep = dt / mDuration;
		Vector3 rotation = rotateStep * mRotateAmount;
		this->mModel->mTransform.Rotate(rotation);

	}else
	{
		//reset data;
		mAccumulateTime = 0.0f;
		mResultState = STATE_SPIN;

		SetSpinIn(-10.125f, { 0,-13.36f,0 }, GetPosition(), { 0,0,-75.f }, GetRotation(), 0.25f, ObjectMovement::AxisZ);

	}
}

void NonDiegeticUI_PotTop::OnStateSpin(float dt)
{
	if (mAccumulateTime <= mDuration)
	{
		//Use Ease Circ
		float easeStart = EaseOut::EaseOutCirc(mAccumulateTime / mDuration);
		mAccumulateTime += dt;
		float easeEnd = EaseOut::EaseOutCirc(mAccumulateTime / mDuration);
		float easeStep = easeEnd - easeStart;

		//‰ñ“]‰^“®
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

		//©g‰ñ“]
		float rotateStep = dt/ mDuration;
		Vector3 rotation = rotateStep * mRotateAmount;
		this->mModel->mTransform.Rotate(rotation);


	}
	else
	{
		//reset data;
		mAccumulateTime = 0.0f;
		//Ÿ‚Ìó‘Ô‚ğİ’è‚·‚é
		mResultState = STATE_FALL;
		//Ÿ‚ÌˆÚ“®ŠÔ‚ÆˆÚ“®‹——£‚ğİ’è
		mDistance = { 0,-2.3f,0.f };
		mDuration = 0.5f;
	}
}

void NonDiegeticUI_PotTop::OnStateFall(float dt)
{
	if (mAccumulateTime <= mDuration)
	{
		//Use Ease Circ
		float easeStart = EaseOut::EaseOutBounce(mAccumulateTime / mDuration);
		mAccumulateTime += dt;
		float easeEnd = EaseOut::EaseOutBounce(mAccumulateTime / mDuration);
		float easeStep = easeEnd - easeStart;

		Vector3 pos = mDistance * easeStep;
		pos += GetPosition();
		SetModelPosition(pos);
	}
	else
	{
		//reset data;
		mAccumulateTime = 0.0f;
		//Ÿ‚Ìó‘Ô‚ğİ’è‚·‚é
		mResultState = STATE_SHAKING;
		mDuration = 2.2f;

	}
}

void NonDiegeticUI_PotTop::OnStateShaking(float dt)
{
	if(mAccumulateTime<mDuration)
	{
		mAccumulateTime += dt;
		mModel->mTransform.Rotate({ 0, 0,   sin(mAccumulateTime * 50.f) });

		if(!isWhiteOut)
		{
			if (mAccumulateTime > 0.2f)
			{
				ScreenFadeEffect::Get()->SetState(ScreenOverlayConfig::STATE_WHITE_OUT);
				ScreenFadeEffect::Get()->SetWhiteDuration(2.f, 5.f);
				isWhiteOut = true;
			}
		}
		

	}else
	{
		//Reset Time
		mAccumulateTime = 0.0f;

		//Reset from start
		mResultState = STATE_RISE;

		//reset object state
		mObjectState = STATE_NONE;

		//reset position&rotation
		ResetPosition();

		isWhiteOut = false;

		ResultManager::Get()->SetActive(true);
	}
}

void NonDiegeticUI_PotTop::ResetPosition()
{
	SetModelPosition(mDefaultPosition);
	SetModelRotation(mDefaultRotation);

}



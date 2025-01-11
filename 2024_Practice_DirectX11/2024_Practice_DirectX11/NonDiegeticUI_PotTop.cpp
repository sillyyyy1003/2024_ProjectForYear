#include "NonDiegeticUI_PotTop.h"
enum ObjectState
{
	STATE_NONE = 0,
	STATE_HOVER = 1,
	STATE_CLICK = 2,
	STATE_RESULT= 3 ,	//äÆê¨ââèo
	STATE_MAX 
};

using namespace DirectX;
using namespace DirectX::SimpleMath;

NonDiegeticUI_PotTop::NonDiegeticUI_PotTop()
{
}

void NonDiegeticUI_PotTop::PreUpdate(float dt)
{
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
		OnStateResult();
		break;
	default:
		return;
	}
}

void NonDiegeticUI_PotTop::OnStateClicked()
{
	InteractiveStaticObject::OnStateClicked();
}

void NonDiegeticUI_PotTop::OnStateHover()
{
	InteractiveStaticObject::OnStateHover();
}

void NonDiegeticUI_PotTop::OnStateNone()
{
	InteractiveStaticObject::OnStateNone();
}

void NonDiegeticUI_PotTop::OnStateResult()
{

}

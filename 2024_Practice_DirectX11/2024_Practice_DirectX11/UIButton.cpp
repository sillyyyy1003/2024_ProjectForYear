#include "UIButton.h"

#include "KInput.h"

enum ButtonState
{
	STATE_NONE,		// Default State
	STATE_HOVER,	// マウスがボタンに置く
	STATE_TRIGGER,	// マウスがリリースされてる
	STATE_PRESS,	// マウスが押したら

};

UIButton::UIButton()
{
}

void UIButton::Init(D2DUIConfig::UIShape shape, D2DUIConfig::FontSize size, const char* objName)
{
	D2D_UIStackContainer::Init(shape, size, objName);
}

void UIButton::Update(float dt)
{
	//Debug Function
	D2D_UIStackContainer::Update(dt);
	//Collider 更新
	UpdateCollider();

	PreUpdate(dt);
	GameUpdate(dt);
	LateUpdate(dt);

}


void UIButton::UpdateCollider()
{
	float centerX = mPosition.x + WIN_WIDTH / 2.f;
	float centerY = WIN_HEIGHT / 2.f - mPosition.y;

	mRect.left = centerX - mScale.x / 2.f;
	mRect.top = centerY - mScale.y / 2.f;
	mRect.right = centerX + mScale.x / 2.f;
	mRect.bottom = centerY + mScale.y / 2.f;

}

void UIButton::PreUpdate(float dt)
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(gD3D->MainWnd(), &cursorPos);

	if (cursorPos.x < mRect.left ||
		cursorPos.x > mRect.right ||
		cursorPos.y < mRect.top ||
		cursorPos.y > mRect.bottom)
	{

		mState = STATE_NONE;
	}
	else
	{
		mState = STATE_HOVER;
		//クリックしたら
		if (KInput::IsKeyPress(VK_LBUTTON))
		{
			mState = STATE_PRESS;
		}
		else if (KInput::IsKeyRelease(VK_LBUTTON))
		{
			mState = STATE_TRIGGER;
		}
	}


}

void UIButton::GameUpdate(float dt)
{
	switch (mState)
	{
	default:
	case STATE_NONE:
		mBackGround->SetBaseColor({ 0,1,0,1 });
		break;
	case STATE_PRESS:
		mBackGround->SetBaseColor({ 1,0,0,1 });
		break;
	case STATE_HOVER:
		mBackGround->SetBaseColor({ 0,0,1,1 });
		break;
	}
}

void UIButton::LateUpdate(float dt)
{
}

bool UIButton::IsPressed()
{
	if (mState == STATE_PRESS)
		return true;

	return false;
}

bool UIButton::IsTrigger()
{
	if (mState == STATE_TRIGGER)
	{
		mState = STATE_NONE;//Rest state
		return true;
	}

	return false;
}

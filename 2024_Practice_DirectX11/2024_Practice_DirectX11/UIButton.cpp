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

	mRect.left = (LONG) (centerX - mScale.x / 2.f);
	mRect.top = (LONG)(centerY - mScale.y / 2.f);
	mRect.right = (LONG)(centerX + mScale.x / 2.f);
	mRect.bottom = (LONG)(centerY + mScale.y / 2.f);

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
		mBackGround->SetBaseColor(mDefaultColor);
		mFontColor = mDefaultFontColor;
		break;
	case STATE_PRESS:
		{
		D2D1::ColorF fontColor = mDefaultColor;
		D2D1::ColorF bgColor = mDefaultFontColor;
		mBackGround->SetBaseColor(bgColor);
		mFontColor = fontColor;
		}
		break;
	case STATE_HOVER:
		{
		D2D1::ColorF fontColor = mDefaultColor;
		D2D1::ColorF bgColor = mDefaultFontColor;
		mBackGround->SetBaseColor(bgColor);
		mFontColor = fontColor;
		}
		break;
	case STATE_TRIGGER:
		{
		D2D1::ColorF fontColor = mDefaultColor;
		D2D1::ColorF bgColor = mDefaultFontColor;
		mBackGround->SetBaseColor(bgColor);
		mFontColor = fontColor;
		}
		break;
	}
}

void UIButton::LateUpdate(float dt)
{
}

void UIButton::LoadSaveData(json data)
{
	mBackGround->LoadSaveData(data, "Graphic");

	//DefaultColor
	mDefaultColor = { data["DefaultColor"][0],data["DefaultColor"][1],data["DefaultColor"][2] ,data["DefaultColor"][3] };

	//Set Base Color
	mBackGround->SetBaseColor(mDefaultColor);

	//Text
	mText = data["Text"];
	mDefaultFontColor = { data["DefaultFontColor"][0],data["DefaultFontColor"][1] ,data["DefaultFontColor"][2] ,data["DefaultFontColor"][3] };
	int num = { data["TextAlignment"] };
	mTextAlignment = static_cast<D2DUIConfig::TextAlignment>(num);
	mFontScale= { data["FontScale"][0],data["FontScale"][1] };

	//Button
	mPosition = { data["Position"][0],data["Position"][1] };
	mScale = { data["Scale"][0],data["Scale"][1] };
	mPadding = { data["Padding"][0],data["Padding"][1],data["Padding"][2],data["Padding"][3] };
}

json UIButton::SaveData()
{
	json data;
	data["Graphic"] = mBackGround->SaveData();
	data["Text"] = mText.c_str();
	data["FontColor"] = { mFontColor.r,mFontColor.g,mFontColor.b,mFontColor.a };
	data["Position"] = { mPosition.x,mPosition.y };
	data["Scale"] = { mScale.x,mScale.y };
	data["TextAlignment"] = static_cast<int>(mTextAlignment);
	data["Padding"] = { mPadding.x,mPadding.y,mPadding.z,mPadding.w };
	data["DefaultFontColor"] = { mDefaultFontColor.r,mDefaultFontColor.g,mDefaultFontColor.b,mDefaultFontColor.a };
	data["DefaultColor"] = { mDefaultColor.r,mDefaultColor.g,mDefaultColor.b,mDefaultColor.a };
	data["FontScale"] = { mFontScale.x,mFontScale.y };
	return data;
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

void UIButton::Levitate()
{
}

void UIButton::SetDefaultColor(D2D1::ColorF color)
{
	mDefaultColor = color;
}

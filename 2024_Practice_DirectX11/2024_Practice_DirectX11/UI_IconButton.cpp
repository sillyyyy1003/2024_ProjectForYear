#include "UI_IconButton.h"
#include "D2D_UIRect.h"
#include "D2D_UIRoundedRect.h"
#include "GameApp.h"
#include "KInput.h"
#include "RenderState.h"
#include "SceneManager.h"

void UI_IconButton::Init(D2DUIConfig::UIShape primitiveKind, const char* filePath, const char* objName)
{
	mButtonIcon = std::make_unique<UI_Square>();
	mButtonIcon->Init(filePath);
	mButtonIcon->LoadDefShader();
	mButtonIcon->SetPosZ(0.2f);

	mObjName = objName;
}

void UI_IconButton::Init(const char* bgTex, const char* iconTex, const char* objName)
{
	
	mButtonIcon = std::make_unique<UI_Square>();
	mButtonIcon->Init(iconTex);
	mButtonIcon->LoadDefShader();
	mButtonIcon->SetPosZ(0.2f);//ÉAÉCÉRÉìÇÕàÍî‘è„

	mBg = std::make_unique<UI_Square>();
	mBg->Init(bgTex);
	mBg->LoadDefShader();
	mBg->SetPosZ(1.0f);	//îwåiÇÕâeÇÃè„

	mShadow = std::make_unique<UI_Square>();
	mShadow->Init(bgTex);
	mShadow->LoadDefShader();
	mShadow->SetPosZ(2.0f);	//âeÇÕàÍî‘Ç§ÇµÇÎ
	mShadow->SetDiffuseColor(ColorConfig::DEFAULT_SHADOW_COLOR);

	mObjName = objName;
}

void UI_IconButton::SetPosition(const DirectX::XMFLOAT2& pos)
{
	//mButtonBackground->SetPosition(pos);
	mBg->SetPosition(pos);
	mButtonIcon->SetPosition(pos);
	UpdateCollider();
	mShadow->SetPosition(pos + mShadowOffset);

}

void UI_IconButton::SetScale(const DirectX::XMFLOAT2& scale)
{
	//mButtonBackground->SetScale(scale);
	mBg->SetScale(scale);
	mButtonIcon->SetScale(scale);
	mShadow->SetScale(scale);
	UpdateCollider();
	
}

bool UI_IconButton::IsTrigger()
{
	if (mState == UIButtonConfig::STATE_TRIGGER)
	{
		return true;
	}

	return false;
}

void UI_IconButton::Update(float dt)
{
#ifdef _DEBUG

	if(ImGui::Begin(mObjName.c_str()))
	{
		float pos[2] = { mPosition.x,mPosition.y };
		ImGui::InputFloat2("ButtonPos", pos);
		mPosition = Vector2(pos);
		SetPosition(mPosition);
		mDefaultPosition = mPosition;

		float scale[2] = { mScale.x,mScale.y };
		ImGui::InputFloat2("ButtonScale", scale);
		mScale=	Vector2(scale);
		SetScale(mScale);

		float IconColor[4] = { mButtonIcon->GetDiffuseColor().x,mButtonIcon->GetDiffuseColor().y,mButtonIcon->GetDiffuseColor().z,mButtonIcon->GetDiffuseColor().w };
		ImGui::ColorEdit4("IconColor", IconColor);
		mButtonIcon->SetDiffuseColor(IconColor);

		float bgColor[4] = { mBg->GetDiffuseColor().x,mBg->GetDiffuseColor().y,mBg->GetDiffuseColor().z,mBg->GetDiffuseColor().w };
		ImGui::ColorEdit4("bgColor", bgColor);
		mBg->SetDiffuseColor(bgColor);

		float shadowOffset[2] = {mShadowOffset.x,mShadowOffset.y};
		ImGui::InputFloat2("shadowOffset", shadowOffset);
		mShadowOffset = { shadowOffset[0],shadowOffset[1] };
	}

	ImGui::End(); 

#endif

	PreUpdate(dt);

	GameUpdate(dt);



}

void UI_IconButton::Draw()
{
	//îwåiï`âÊ
	//mButtonBackground->Draw();
	//ÉAÉCÉRÉìï`âÊ
	GameApp::SetBlendState(RenderState::BSTransparent);
	//âeï`âÊ
	if (mState != UIButtonConfig::STATE_NONE)mShadow->Draw();
	//îwåiï`âÊ
	mBg->Draw();
	//ICONï`âÊ
	mButtonIcon->Draw();
	GameApp::SetBlendState(nullptr);
}

json UI_IconButton::SaveData()
{
	json data;
	data["Position"] = { mPosition.x,mPosition.y };
	data["Scale"] = { mScale.x,mScale.y };
	data["ShadowOffset"] = { mShadowOffset.x,mShadowOffset.y };

	return data;

}

void UI_IconButton::LoadSaveData(json data)
{
	mPosition = { data["Position"][0],data["Position"][1] };
	mScale = { data["Scale"][0],data["Scale"][1] };
	mShadowOffset = { data["ShadowOffset"][0],data["ShadowOffset"][1] };

	SetPosition(mPosition);
	SetScale(mScale);
	mDefaultPosition = mPosition;
}

void UI_IconButton::PreUpdate(float dt)
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(gD3D->MainWnd(), &cursorPos);

	if (cursorPos.x < mRect.left ||
		cursorPos.x > mRect.right ||
		cursorPos.y < mRect.top ||
		cursorPos.y > mRect.bottom)
	{

		mState = UIButtonConfig::STATE_NONE;
	}
	else
	{
		mState = UIButtonConfig::STATE_HOVER;
		//ÉNÉäÉbÉNÇµÇΩÇÁ
		if (KInput::IsKeyPress(VK_LBUTTON))
		{
			mState = UIButtonConfig::STATE_PRESS;
		}
		else if (KInput::IsKeyRelease(VK_LBUTTON))
		{
			mState = UIButtonConfig::STATE_TRIGGER;
		}
	}
}

void UI_IconButton::GameUpdate(float dt)
{
	switch (mState)
	{
	default:
	case UIButtonConfig::STATE_NONE:
		OnStateNone(dt);
		break;
	case UIButtonConfig::STATE_PRESS:
		OnStatePress(dt);
		break;
	case UIButtonConfig::STATE_HOVER:
		OnStateHover(dt);
		break;
	case UIButtonConfig::STATE_TRIGGER:
		OnStateTrigger(dt);
	break;
	}
}

void UI_IconButton::OnStateNone(float dt)
{
	SetPosition(mDefaultPosition);
}

void UI_IconButton::OnStateHover(float dt)
{
	SetPosition({ mDefaultPosition.x,mDefaultPosition.y + mPositionOffset});
}

void UI_IconButton::OnStatePress(float dt)
{
}

void UI_IconButton::OnStateTrigger(float dt)
{
	
}

void UI_IconButton::UpdateCollider()
{
	float centerX = mPosition.x + (float)GameApp::Get()->GetWinWidth() / 2.f;
	float centerY = (float)GameApp::Get()->GetWinHeight() / 2.f - mPosition.y;

	mRect.left = (LONG)(centerX - mScale.x / 2.f);
	mRect.top = (LONG)(centerY - mScale.y / 2.f);
	mRect.right = (LONG)(centerX + mScale.x / 2.f);
	mRect.bottom = (LONG)(centerY + mScale.y / 2.f);
}

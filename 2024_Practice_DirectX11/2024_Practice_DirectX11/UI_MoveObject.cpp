#include "UI_MoveObject.h"
#include "GameApp.h"
#include "KInput.h"
#include "D3DUtil.h"

void UI_MoveObject::Init(const char* _objName, const std::shared_ptr<Texture>& iconTex, const std::shared_ptr<Texture>& fontTex)
{
	//Set Icon Tex
	mGraphic = std::make_unique<UIStackContainer>();
	mGraphic->InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind::SQUARE);
	mGraphic->LoadBackgroundTex(iconTex, { 1,1 });
	mGraphic->LoadFontTexture(fontTex, UITextOption::FONT_DEFAULT_SIZE);

	//Set Obj Name
	SetObjName(_objName);
}

void UI_MoveObject::Update(float dt)
{
	mGraphic->Update();
	PreUpdate(dt);
	GameUpdate(dt);
}


void UI_MoveObject::Draw()
{
	mGraphic->Draw();
}

void UI_MoveObject::PreUpdate(float dt)
{

	//今のボタンの位置を計算する
	float winHeight = static_cast<float>(GameApp::Get()->GetWinHeight());
	float winWidth = static_cast<float>(GameApp::Get()->GetWinWidth());
	float buttonWidth = mGraphic->GetContainerSize().x;
	float buttonHeight = mGraphic->GetContainerSize().y;
	float posX = mGraphic->GetPosition().x + winWidth / 2;
	float posY = abs(mGraphic->GetPosition().y - winHeight / 2);//width/2~-width/2

	float left = posX - buttonWidth / 2.f;
	float top = posY - buttonHeight / 2.f;
	float right = posX + buttonWidth / 2.f;
	float bottom = posY + buttonHeight / 2.f;

	//Get Cursor Pos;
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	//Transform cursorPos to Client pos
	ScreenToClient(gD3D->MainWnd(), &cursorPos);

	switch(mState)
	{
		default:
	case UI_MoveObjectConfig::UiMoveState::STATE_NONE:
		if (cursorPos.x > left &&
			cursorPos.x < right &&
			cursorPos.y > top &&
			cursorPos.y < bottom)
		{
			if (KInput::IsKeyPress(VK_LBUTTON))
			{
				mState = UI_MoveObjectConfig::UiMoveState::STATE_DRAG;
			}else
			{
				mState = UI_MoveObjectConfig::UiMoveState::STATE_HOVER;
			}
		
		}
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_HOVER:
		
		if (KInput::IsKeyPress(VK_LBUTTON))
		{
			mState = UI_MoveObjectConfig::UiMoveState::STATE_DRAG;
		}else
		{
			mState = UI_MoveObjectConfig::UiMoveState::STATE_NONE;
		}
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_DRAG:
		
		if (KInput::IsKeyPress(VK_LBUTTON))
		{

		}else
		{
			mState = UI_MoveObjectConfig::UiMoveState::STATE_RELEASE;
		}
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_RELEASE:

		mState = UI_MoveObjectConfig::UiMoveState::STATE_NONE;

		break;
	}

}

void UI_MoveObject::GameUpdate(float dt)
{
	switch(mState)
	{
	default:
	case UI_MoveObjectConfig::UiMoveState::STATE_NONE:
		OnStateNone(dt);
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_DRAG:
		OnStateDrag(dt);
		break;
	case UI_MoveObjectConfig::UiMoveState::STATE_HOVER:
		OnStateHover(dt);
	case UI_MoveObjectConfig::UiMoveState::STATE_RELEASE:
		OnStateRelease(dt);
		break;
	}
}

json UI_MoveObject::SaveData()
{
	json data;
	//Graphicの部分のデータ
	data = mGraphic->SaveData();
	return data;
}

void UI_MoveObject::LoadSaveData(json saveData)
{
	//Graphicの部分
	mGraphic->LoadSaveData(saveData, "Graphic");
}

void UI_MoveObject::OnStateNone(float dt)
{
	isRelease = false;
	
}

void UI_MoveObject::OnStateHover(float dt)
{
}

void UI_MoveObject::OnStateDrag(float dt)
{
	//GetCursor Pos

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(gD3D->MainWnd(), &cursorPos);

	float winHeight = static_cast<float>(GameApp::Get()->GetWinHeight());
	float winWidth = static_cast<float>(GameApp::Get()->GetWinWidth());

	DirectX::XMFLOAT2 objPos = LocalClientToWorld(cursorPos,{winWidth,winHeight});
	mGraphic->SetPosition(objPos.x, objPos.y);
}

void UI_MoveObject::OnStateRelease(float dt)
{
	isRelease = true;
}

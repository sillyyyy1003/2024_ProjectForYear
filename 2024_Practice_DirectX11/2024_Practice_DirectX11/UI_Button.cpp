#include "UI_Button.h"
#include "GameApp.h"
#include "KInput.h"
#include "Sprite.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

enum ButtonState
{
	STATE_NONE,		// Default State
	STATE_HOVER,	// �}�E�X���{�^���ɒu��
	STATE_TRIGGER,	// �}�E�X�������[�X����Ă�
	STATE_PRESS,	// �}�E�X����������

};

UI_Button::~UI_Button()
{
}


void UI_Button::Draw(int texSlot)
{
	mContainer->Draw();

}

bool UI_Button::isPressed()
{
	if (mState == STATE_PRESS)
		return true;

	return false;
}

bool UI_Button::isTrigger()
{
	if (mState == STATE_TRIGGER)
	{
		mState = STATE_NONE;//Rest state
		return true;
	}

	return false;
}

void UI_Button::LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	
}

void UI_Button::Init(UIPrimitiveConfig::UI_PrimitiveKind primitiveKind, const std::shared_ptr<Texture>& bgTex, DirectX::XMFLOAT2 containerSize, const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 charSize)
{
	mContainer = std::make_unique<UIStackContainer>();
	mContainer->InitUIStackContainer(primitiveKind);
	mContainer->LoadBackgroundTex(bgTex, containerSize);
	mContainer->LoadFontTexture(fontTex, charSize);

	//Init StateColor
	for (int i = 0; i < UIButton::STATE_MAX; i++)
	{
		mStateColor[i] = UIButton::DEFAULT_BUTTON_STATE_COLOR[i];
	}
}

void UI_Button::Init(UIPrimitiveConfig::UI_PrimitiveKind primitiveKind, const std::shared_ptr<Texture>& bgTex,
	const std::shared_ptr<Texture>& fontTex)
{
	mContainer = std::make_unique<UIStackContainer>();
	mContainer->InitUIStackContainer(primitiveKind);
	mContainer->LoadBackgroundTex(bgTex, { 1,1 });
	mContainer->LoadFontTexture(fontTex, { 1,1 });
	for (int i = 0; i < UIButton::STATE_MAX; i++)
	{
		mStateColor[i] = UIButton::DEFAULT_BUTTON_STATE_COLOR[i];
	}
}

void UI_Button::SetStateColor(DirectX::XMFLOAT4 color, UIButton::ButtonState state)
{
	mStateColor[state] = color;
}

void UI_Button::Update()
{
	mContainer->Update();

	PreUpdate();
	GameUpdate();
}

void UI_Button::PreUpdate()
{
	//���̃{�^���̈ʒu���v�Z����
	float winHeight = static_cast<float>(GameApp::Get()->GetWinHeight());
	float winWidth = static_cast<float>(GameApp::Get()->GetWinWidth());

	float buttonWidth = mContainer->GetContainerSize().x;
	float buttonHeight = mContainer->GetContainerSize().y;
	
	float posX = mContainer->GetPosition().x + winWidth / 2;
	float posY = abs(mContainer->GetPosition().y - winHeight / 2);//width/2~-width/2

	float left = posX - buttonWidth / 2.f;
	float top = posY - buttonHeight / 2.f;
	float right = posX + buttonWidth / 2.f;
	float bottom = posY + buttonHeight / 2.f;

	//Get Cursor Pos;
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	//Transform cursorPos to Client pos
	ScreenToClient(gD3D->MainWnd(), &cursorPos);

	if (cursorPos.x < left ||
		cursorPos.x > right ||
		cursorPos.y < top ||
		cursorPos.y > bottom)
	{

		mState = STATE_NONE;
	}
	else
	{
		mState = STATE_HOVER;
		//�N���b�N������
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


void UI_Button::GameUpdate()
{
	//todo:change
	switch (mState)
	{
	default:
	case STATE_NONE:
		mContainer->mUiSet.mBackGround->SetAmbientColor(mStateColor[UIButton::STATE_NONE]);
		break;
	case STATE_PRESS:
		mContainer->mUiSet.mBackGround->SetAmbientColor(mStateColor[UIButton::STATE_PRESS]);
		break;
	case STATE_HOVER:
		mContainer->mUiSet.mBackGround->SetAmbientColor(mStateColor[UIButton::STATE_HOVER]);
		break;
	}

}

void UI_Button::SetPosZ(float Z)
{
	mContainer->SetPosZ(Z);
}


void UI_Button::LoadSaveData(json data, const char* objName)
{
	mContainer->LoadSaveData(data, objName);
}

json UI_Button::SaveData(const char* objName)
{
	json data;
	data = mContainer->SaveData(objName);
	return data;
}
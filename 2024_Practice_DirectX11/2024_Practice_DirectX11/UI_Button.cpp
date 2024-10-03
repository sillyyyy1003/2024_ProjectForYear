#include "UI_Button.h"

#include "GampApp.h"
#include "GUI.h"
#include "KInput.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

enum ButtonState
{
	STATE_NONE,		// Default State
	STATE_TOUCH,	// マウスがボタンに置く
	STATE_TRIGGER,	// マウスがリリースされてる
	STATE_PRESS,	// マウスが押したら
	
};

UI_Button::~UI_Button()
{
}

void UI_Button::Init(const char* _fileName)
{
	CreateMeshBuffer();
	CreateMaterial(_fileName);
	LoadShader();
}

void UI_Button::Update(float dt)
{
	PreUpdate(dt);
	
	GameUpdate(dt);

	LateUpdate(dt);

}

void UI_Button::Draw()
{


	if(isDefShader)
	{
		mVS = mDefVS.get();
		mPS = mDefPS.get();
	}

	mVS->SetShader();
	mPS->SetShader();

	if (mMaterial.tex)
		mDefPS->SetTexture(0, mMaterial.tex.get());
	mMesh->Draw();
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

void UI_Button::WriteShader()
{
	Vector2 viewSize = { static_cast<float>(gD3D->GetWinWidth()),static_cast<float>(gD3D->GetWinHeight()) };

	XMMATRIX WVP[3] = {};
	WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
	WVP[2] = XMMatrixOrthographicLH(viewSize.x, viewSize.y, 0.0f, 3.0f);
	WVP[2] = XMMatrixTranspose(WVP[2]);

	//Write Shader
	mDefVS->WriteShader(0, WVP);
	mDefPS->WriteShader(0, &(mMaterial.material));
	mDefPS->WriteShader(1, &mEffect);

}

void UI_Button::LoadShader()
{
	mDefPS = std::make_unique<PixelShader>();
	mDefVS = std::make_unique<VertexShader>();

	HR(mDefPS->LoadShaderFile("Assets/Shader/PS_ButtonUI.cso"));
	HR(mDefVS->LoadShaderFile("Assets/Shader/VS_DefaultUI.cso"));
}

void UI_Button::PreUpdate(float dt)
{
	//window sizeによるボタンサイズの更新
	CanvasUI::Update(dt);

	//今のボタンの位置を計算する
	float winHeight = static_cast<float>(GameApp::Get()->GetWinHeight());
	float winWidth = static_cast<float>(GameApp::Get()->GetWinWidth());

	float buttonWidth = mTransform.GetScale().x;
	float buttonHeight = mTransform.GetScale().y;

	float posX = mTransform.GetPosition().x + winWidth / 2;
	float posY = abs(mTransform.GetPosition().y - winHeight / 2);//width/2~-width/2

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
		cursorPos.y > bottom) {

		mState = STATE_NONE;
	}
	else 
	{
		mState = STATE_TOUCH;
		//クリックしたら
		if (KInput::IsKeyPress(VK_LBUTTON))
		{
			mState = STATE_PRESS;
		}
		else if(KInput::IsKeyRelease(VK_LBUTTON))
		{
			mState = STATE_TRIGGER;
		}
	}

}


void UI_Button::GameUpdate(float dt)
{
	
	switch(mState)
	{
	default:
	case STATE_NONE:
		mEffect = Color(0.85f, 0.85f, 0.85f, 1.0f);
		break;
	case STATE_PRESS:
		mEffect = Color(1.f, 0.4f, 0.2f, 1.0f);
		break;
	case STATE_TOUCH:
		mEffect = Color(1.f, 1.f, 1.f, 1.0f);
		break;
	}

}

void UI_Button::LateUpdate(float dt)
{
	WriteShader();
}

RECT UI_Button::GetRect()
{
	return mRect;
}

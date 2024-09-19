#include "Primitive.h"
#include "KInput.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


enum DebugState
{
	STATE_NONE,
	STATE_CHECK,
	//todo:追加物体移动
};

Primitive::Primitive(PrimitiveKind kind) :mKind(kind)
{
}

Primitive::~Primitive()
{
}

void Primitive::Update(float dt)
{
	/*
	UpdateState();
	if (mState == STATE_NONE) { return; }

	//オブジェクトを回転させる(xy軸だけ)
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	//XMFLOAT2 mouseMove = DirectX::XMFLOAT2((float)cursorPos.x - mOldPos.x, (float)cursorPos.y - mOldPos.y);
	XMFLOAT2 mouseMove = DirectX::XMFLOAT2((float)mOldPos.x-cursorPos.x, (float)mOldPos.y-cursorPos.y);
	mOldPos = cursorPos;

	switch (mState)
	{
	case STATE_CHECK:	
		UpdateTransform(mouseMove, dt);	break;
	default:;
	}
	*/

}

void Primitive::SetScaleXZ(const DirectX::XMFLOAT2& scale)
{
	mTransform.SetScale(scale.x, 1.0f, scale.y);
}

void Primitive::LoadDefShader()
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();

	mDefPS->LoadShaderFile("Assets/Shader/PS_Primitives.cso");
	mDefVS->LoadShaderFile("Assets/Shader/VS_Primitives.cso");
}

void Primitive::UpdateState()
{
	/*
	DebugState prev = static_cast<DebugState>(mState);

	if(KInput::IsKeyPress(VK_CONTROL))
	{
		mState = STATE_NONE;

	}if (KInput::IsKeyPress(VK_LBUTTON))
	{
		mState = STATE_CHECK;
	}
	else
	{
		mState = STATE_NONE;
	}

	if (prev != mState)
		GetCursorPos(&mOldPos);*/
}

void Primitive::UpdateTransform(DirectX::XMFLOAT2 mouseMove, float dt)
{
 
}

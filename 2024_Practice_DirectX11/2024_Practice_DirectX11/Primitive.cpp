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


void Primitive::SetScaleXZ(const DirectX::XMFLOAT2& scale) noexcept
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

void Primitive::LoadDefShader(const char* vsPath, const char* psPath)
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();
	mDefPS->LoadShaderFile(psPath);
	mDefVS->LoadShaderFile(vsPath);

}

void Primitive::ResetDefPSShader()
{
	mDefPS.reset();
	mDefPS = std::make_shared<PixelShader>();
}

void Primitive::ResetDefVSShader()
{
	mDefVS.reset();
	mDefVS = std::make_shared<VertexShader>();
}

void Primitive::SetDefShader()
{
	if (isDefShader)
	{
		mVS = mDefVS.get();
		mPS = mDefPS.get();
	}
}

void Primitive::SetVertices(std::vector<Vertex::VtxPosNormalTex> vertices) noexcept
{
	mVertices = vertices;
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

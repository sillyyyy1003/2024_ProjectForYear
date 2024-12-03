#include "UI_RoundSquare.h"

UI_RoundSquare::UI_RoundSquare():UI_Primitive(UIPrimitiveConfig::UI_PrimitiveKind::ROUND_SQUARE)
{
}

UI_RoundSquare::~UI_RoundSquare()
{
}

void UI_RoundSquare::Init(const std::shared_ptr<Texture>& pTex, DirectX::XMFLOAT2 _squareSize, float radius, int slices,
	DirectX::XMFLOAT2 _UVSplit)
{
	mRoundRadius = radius;
	mSquareSize = _squareSize;

	mUvAnimation = std::make_unique<UVAnimation>();

}

void UI_RoundSquare::Update()
{
	UI_Primitive::Update();
}

void UI_RoundSquare::Draw(int texSlot)
{
	SetDefShader();
	mVS->SetShader();
	mPS->SetShader();
	if (mMaterial.tex)
		mDefPS->SetTexture(0, mMaterial.tex.get());
	mMesh->Draw();
}

void UI_RoundSquare::CreateMesh()
{
	
}

void UI_RoundSquare::WriteDefShader()
{

}

#include "UVAnimation.h"

UVAnimation::UVAnimation()
{
}

UVAnimation::~UVAnimation()
{
}

void UVAnimation::Init(DirectX::XMINT2 split)
{
	mSplit = split;
	int animeNum = split.x * split.y;

	for (int i = 0; i < animeNum; i++)
	{
		mAnimeTable.push_back(i);
	}

	//SetEndFrame
	mAnimeTable.push_back(-1);

}

void UVAnimation::SetOffsetUV(DirectX::XMFLOAT2 _offsetUV)
{
}

void UVAnimation::SetOffsetUV(float offsetU, float offsetV)
{
}

void UVAnimation::UpdateUV()
{
	mOffsetUV.x = 1.0f / mSplit.x * mFrame.x;
	mOffsetUV.y = 1.0f / mSplit.y * mFrame.y;
}

#pragma once
#include "D3DUtil.h"

namespace
{
	enum class AnimePattern
	{
		Null,
	};
}


/// @brief UV Animation
class UVAnimation
{
protected:
	DirectX::XMFLOAT2 mOffsetUV={0,0};

	DirectX::XMINT2 mSplit = { 1,1 };
	float mAnimeCounter = 0.0f;

	float mAnimeSpeed = 0.0f;

	bool isLoop = false;
	bool isPlaying = false;

	DirectX::XMINT2 mFrame{ 0,0 };//ç°ÇÕÇ«ÇÃÉtÉåÅ[ÉÄ

	std::vector<int> mAnimeTable;

public:
	UVAnimation();
	~UVAnimation();

	virtual void Init(DirectX::XMINT2 split);

	void SetLoop(bool _isLoop) { isLoop = _isLoop; };
	void SetIsPlaying(bool _isPlaying) { isPlaying = _isPlaying; };

	bool GetLoop() const { return isLoop; };
	bool GetIsPlaying() const { return isPlaying; };

	void SetOffsetUV(DirectX::XMFLOAT2 _offsetUV);
	void SetOffsetUV(float offsetU, float offsetV);

	void SetFrameX(int frameX) { mFrame.x = frameX; };
	void SetFrameY(int frameY) { mFrame.y = frameY; };

	DirectX::XMFLOAT2 GetOffsetUV() const { return mOffsetUV; };
	DirectX::XMINT2 GetSplit() const { return mSplit; };

	void UpdateUV();
};


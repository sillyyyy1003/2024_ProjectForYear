#pragma once
#include "UI_Primitive.h"

namespace ScreenOverlayConfig
{
	enum OverlayState
	{
		STATE_NONE,
		STATE_FADE_IN,
		STATE_FADE_OUT,
		STATE_WHITE_IN,
		STATE_WHITE_OUT,
		
	};

	constexpr float FADE_SPEED = 1.0f;
}
//ScreenOver Function
class ScreenFadeEffect
{
private:

	struct WhiteFadeCBuffer
	{
		DirectX::XMFLOAT2 ScreenCenter = { 0.5f,0.5f };
		float intensity;
		float radius;
	};

	int mState = 0;

	//Fade in/out 表示用
	std::unique_ptr<UI_Primitive> mFade;

	//White in/out　表示
	std::unique_ptr<UI_Primitive> mWhite;

	float mFadeParam = 1.0f;

	float mInDuration = 0.0f;
	float mOutDuration = 0.0f;

	float mAccumulateTime = 0.0f;
	WhiteFadeCBuffer mCBuffer = {};

	ScreenFadeEffect();
	~ScreenFadeEffect();

public:

	static ScreenFadeEffect* Get() { static ScreenFadeEffect instance; return &instance; }

	void Init();
	void Update(float dt);

	void SetDefaultState();
	void SetState(ScreenOverlayConfig::OverlayState state);

	/// @brief 演出の時間を設定
	/// @param outDuration 
	/// @param inDuration 
	void SetWhiteDuration(float outDuration,float inDuration);

	/// @brief LayerDepthを設定する
	/// @param posZ 
	void SetPosZ(float posZ);

	void Draw();


	void FadeOut(float dt);
	void FadeIn(float dt);
	void WhiteOut(float dt);
	void WhiteIn(float dt);

	bool GetFadeOut();
	bool GetFadeIn();

	bool GetFade();

	

};


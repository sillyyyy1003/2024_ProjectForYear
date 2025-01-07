#pragma once
#include "UI_Primitive.h"

namespace ScreenOverlayConfig
{
	enum OverlayState
	{
		STATE_NONE,
		STATE_FADE_IN,
		STATE_FADE_OUT,

	};

	constexpr float FADE_SPEED = 1.0f;
}
//ScreenOver Function
class ScreenFadeEffect
{
private:

	int mState = 0;

	//Fade in/out 表示用
	std::unique_ptr<UI_Primitive> mFade;

	float mFadeParam = 1.0f;

	ScreenFadeEffect();
	~ScreenFadeEffect();

public:

	static ScreenFadeEffect* Get() { static ScreenFadeEffect instance; return &instance; }

	void Init();
	void Update(float dt);

	void SetDefaultState();
	void SetState(ScreenOverlayConfig::OverlayState state);

	/// @brief LayerDepthを設定する
	/// @param posZ 
	void SetPosZ(float posZ);

	void Draw();


	void FadeOut(float dt);
	void FadeIn(float dt);

	bool GetFadeOut();
	bool GetFadeIn();

	bool GetFade();

};


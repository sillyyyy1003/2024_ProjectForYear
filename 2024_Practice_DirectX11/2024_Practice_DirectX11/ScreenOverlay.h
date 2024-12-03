#pragma once
#include "UI_Primitive.h"

//ScreenOver Function
class ScreenOverlay
{
	std::unique_ptr<UI_Primitive> mOverlay;

public:

	ScreenOverlay();
	~ScreenOverlay();

	void Init();
	void Update();

	void ScreenOverlayEffect();
};


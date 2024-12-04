#pragma once
#include "UI_Primitive.h"

//ScreenOver Function
class ScreenOverlay
{
private:

	std::unique_ptr<UI_Primitive> mOverlay;
	ScreenOverlay();
	~ScreenOverlay();
public:

	static ScreenOverlay* Get() { static ScreenOverlay instance; return &instance; }

	void Init();
	void Update();

	
	void ScreenOverlayEffect();

	/// @brief LayerDepth‚ğİ’è‚·‚é
	/// @param posZ 
	void SetPosZ(float posZ);

};


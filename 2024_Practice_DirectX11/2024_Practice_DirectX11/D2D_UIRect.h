#pragma once
#include "D2D_UIBase.h"

class D2D_UIRect:public D2D_UIBase
{
public:
	D2D_UIRect();

	/// @brief Draw with solid brushes
	void Draw() override;

	/// @brief Draw with radian brushes
	///	No outline
	void DrawWithRadianBrush() override;
};


#include "D2D_UIRoundedRect.h"

D2D_UIRoundedRect::D2D_UIRoundedRect():D2D_UIBase(D2DUIConfig::UIShape::ROUNDED_RECT)
{
}

void D2D_UIRoundedRect::Draw()
{
	float centerX = mPos.x + WIN_WIDTH / 2.f;
	float centerY = WIN_HEIGHT / 2.f - mPos.y;
	D2D1_RECT_F rect;
	rect.left = centerX - mScale.x / 2.f;
	rect.top = centerY - mScale.y / 2.f;
	rect.right = centerX + mScale.x / 2.f;
	rect.bottom = centerY + mScale.y / 2.f;

	D2D1_RECT_F outlineRect = {
		rect.left - mOutlineWeight,
		rect.top - mOutlineWeight,
		rect.right + mOutlineWeight,
		rect.bottom + mOutlineWeight
	};

	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();
	//Draw Outline first
	D2DBrush::Get()->GetSolidBrush()->SetColor(mOutlineColor);
	renderTarget->FillRoundedRectangle({ outlineRect,mRadiusX	,mRadiusY}, D2DBrush::Get()->GetSolidBrush());
	//Draw Content
	D2DBrush::Get()->GetSolidBrush()->SetColor(mBaseColor);
	renderTarget->FillRoundedRectangle({ rect,mRadiusX,mRadiusY }, D2DBrush::Get()->GetSolidBrush());
	renderTarget->EndDraw();
}



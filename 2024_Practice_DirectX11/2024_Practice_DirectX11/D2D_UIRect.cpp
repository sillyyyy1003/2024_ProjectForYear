#include "D2D_UIRect.h"


D2D_UIRect::D2D_UIRect():D2D_UIBase(D2DUIConfig::UIShape::RECT)
{
}

void D2D_UIRect::Draw()
{
	float centerX = mPos.x + WIN_WIDTH / 2.f;
	float centerY = WIN_HEIGHT / 2.f - mPos.y;
	D2D1_RECT_F rect;
	rect.left = centerX -mScale.x / 2.f;
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
	renderTarget->FillRectangle(outlineRect, D2DBrush::Get()->GetSolidBrush());
	//Draw Content
	D2DBrush::Get()->GetSolidBrush()->SetColor(mBaseColor);
	renderTarget->FillRectangle(rect, D2DBrush::Get()->GetSolidBrush());
	renderTarget->EndDraw();
}

void D2D_UIRect::DrawWithRadianBrush()
{
	float centerX = mPos.x + WIN_WIDTH / 2.f;
	float centerY = WIN_HEIGHT / 2.f - mPos.y;
	D2D1_RECT_F rect;
	rect.left = centerX - mScale.x / 2.f;
	rect.top = centerY - mScale.y / 2.f;
	rect.right = centerX + mScale.x / 2.f;
	rect.bottom = centerY + mScale.y / 2.f;

	
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();

	//Draw Content
	renderTarget->FillRectangle(rect, D2DBrush::Get()->GetRGBrush());
	renderTarget->EndDraw();
}

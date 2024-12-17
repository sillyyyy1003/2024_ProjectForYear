#include "D2DBrush.h"

#include "D2DFont.h"
#include "D3DApp.h"


void D2DBrush::InitBrush()
{
	InitSolidBrush();
	InitLinearBrush();
	InitRadianBrush();

	mBrushList.emplace(D2DBrushConfig::BrushKind::SOLID, GetSolidBrush());
	mBrushList.emplace(D2DBrushConfig::BrushKind::LINEAR, GetLGBrush());
	mBrushList.emplace(D2DBrushConfig::BrushKind::RADIAN, GetRGBrush());

}

void D2DBrush::SetSolidBrushColor(D2D1::ColorF color)
{
	mSolidBrush->SetColor(color);
}

void D2DBrush::SetBrushColor(D2D1::ColorF frontColor, D2D1::ColorF backColor, D2DBrushConfig::BrushKind brush)
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	mFrontColorF = frontColor;
	mBackColorF = backColor;

	switch(brush)
	{
	case D2DBrushConfig::BrushKind::LINEAR:
		InitLinearBrush();
		break;
	case D2DBrushConfig::BrushKind::RADIAN:
		InitRadianBrush();
		break;
	}
}


void D2DBrush::UnInit()
{
	mSolidBrush.Reset();		//Mono tone
	mRGBrush.Reset();			//Radian
	mLGBrush.Reset();			//Linear
	mGradientStopLiner.Reset();
	mGradientStopRadian.Reset();
}

void D2DBrush::InitSolidBrush()
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	
	//Create Solid Brush
	renderTarget->CreateSolidColorBrush(mFrontColorF, mSolidBrush.GetAddressOf());
}

void D2DBrush::InitLinearBrush()
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();

	//Create Linear Gradient Brush
	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0] = { 0.0f, mFrontColorF };
	gradientStops[1] = { 1.0f, mBackColorF };

	renderTarget->CreateGradientStopCollection(gradientStops, 2, mGradientStopLiner.GetAddressOf());

	renderTarget->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0),
			D2D1::Point2F(100, 100)),
		mGradientStopLiner.Get(),
		mLGBrush.GetAddressOf());
}

void D2DBrush::InitRadianBrush()
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	//Create Linear Gradient Brush
	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0] = { 0.0f, mFrontColorF };
	gradientStops[1] = { 1.0f, mBackColorF };
	//Create Radian Gradiant Brush
	renderTarget->CreateGradientStopCollection(gradientStops, 2, mGradientStopRadian.GetAddressOf());
	renderTarget->CreateRadialGradientBrush(
		D2D1::RadialGradientBrushProperties(
			D2D1::Point2F(50, 50),
			D2D1::Point2F(0, 0),
			50, 50),
		mGradientStopRadian.Get(),
		mRGBrush.GetAddressOf());
}

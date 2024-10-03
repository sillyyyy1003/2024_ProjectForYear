#include "D2DBrush.h"
#include "D3DApp.h"


D2DBrush::~D2DBrush()
{
}

void D2DBrush::InitBrush()
{
	InitSolidBrush();
	InitLinearBrush();
	InitRadianBrush();

	mBrushList.emplace(SOLID, GetSolidBrush());
	mBrushList.emplace(LINEAR, GetLGBrush());
	mBrushList.emplace(RADIAN, GetRGBrush());


}

void D2DBrush::SetSolidBrushColor(D2D1::ColorF color)
{
	mSolidBrush->SetColor(color);
}

void D2DBrush::SetBrushColor(D2D1::ColorF frontColor, D2D1::ColorF backColor, BrushKind brush)
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	mFrontColorF = frontColor;
	mBackColorF = backColor;

	switch(brush)
	{
	case LINEAR:
		InitLinearBrush();
		break;
	case RADIAN:
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

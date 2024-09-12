#pragma once
#include <d2d1.h>
#include <map>
#include <string>
#include <wrl/client.h>
using namespace Microsoft::WRL;

enum BrushKind
{
	SOLID,
	LINEAR,
	RADIAN,
};
/// @brief D2D Render�pBrush
class D2DBrush
{
private:

	ComPtr<ID2D1SolidColorBrush> mSolidBrush;		//Mono tone
	ComPtr<ID2D1RadialGradientBrush> mRGBrush;		//Radian
	ComPtr<ID2D1LinearGradientBrush> mLGBrush;		//Linear
	//ComPtr<ID2D1BitmapBrush> mBitMapBrush = nullptr;		//ImageBrush

	D2D1::ColorF mFrontColorF = D2D1::ColorF::White;		//�O�i�F
	D2D1::ColorF mBackColorF = D2D1::ColorF::Black;			//�@�i�F

	ComPtr<ID2D1GradientStopCollection> mGradientStopLiner = nullptr;
	ComPtr<ID2D1GradientStopCollection> mGradientStopRadian= nullptr;

public:
	D2DBrush() {};
	~D2DBrush();

	ID2D1SolidColorBrush* GetSolidBrush() { return mSolidBrush.Get(); };		//Mono tone
	ID2D1RadialGradientBrush* GetRGBrush() { return mRGBrush.Get(); };		//Radian
	ID2D1LinearGradientBrush* GetLGBrush() { return mLGBrush.Get(); };

	/// @brief Brush������
	void InitBrush();

	/// @brief SolidBrush Color��ݒ肷��
	///	@param color �ݒ肷��F
	void SetBrushColor(D2D1::ColorF color);

	/// @brief
	///	@param
	///	@param 
	void SetBrushColor(D2D1::ColorF frontColor, D2D1::ColorF backColor,BrushKind brush);

	void UnInit();
private:

	void InitSolidBrush();
	void InitLinearBrush();
	void InitRadianBrush();
};


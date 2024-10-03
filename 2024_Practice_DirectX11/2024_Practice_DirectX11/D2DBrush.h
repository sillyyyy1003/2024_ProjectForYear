#pragma once
#include <d2d1.h>
#include <map>
#include <string>
#include <variant>
#include <wrl/client.h>
using namespace Microsoft::WRL;

enum BrushKind
{
	SOLID,
	LINEAR,
	RADIAN,
};
/// @brief D2D Render用Brush
class D2DBrush
{
private:

	ComPtr<ID2D1SolidColorBrush> mSolidBrush;		//Mono tone
	ComPtr<ID2D1RadialGradientBrush> mRGBrush;		//Radian
	ComPtr<ID2D1LinearGradientBrush> mLGBrush;		//Linear
	//ComPtr<ID2D1BitmapBrush> mBitMapBrush = nullptr;		//ImageBrush

	D2D1::ColorF mFrontColorF = D2D1::ColorF::White;		//前景色
	D2D1::ColorF mBackColorF = D2D1::ColorF::Black;			//后景色

	ComPtr<ID2D1GradientStopCollection> mGradientStopLiner = nullptr;
	ComPtr<ID2D1GradientStopCollection> mGradientStopRadian= nullptr;
	using BrushVariant = std::variant<ID2D1SolidColorBrush*, ID2D1LinearGradientBrush*, ID2D1RadialGradientBrush*>;
	using BrushList = std::map<BrushKind, ID2D1Brush*>;

public:
	BrushList mBrushList;

public:

	D2DBrush() {};
	~D2DBrush();

	ID2D1SolidColorBrush* GetSolidBrush() { return mSolidBrush.Get(); };		//Mono tone
	ID2D1RadialGradientBrush* GetRGBrush() { return mRGBrush.Get(); };		//Radian
	ID2D1LinearGradientBrush* GetLGBrush() { return mLGBrush.Get(); };

	/// @brief Brush初期化
	void InitBrush();

	/// @brief SolidBrush Colorを設定する
	///	@param color 設定する色
	void SetSolidBrushColor(D2D1::ColorF color);

	/// @brief Radian& Linear Brushの色を設定する
	/// @param frontColor 前景色
	/// @param backColor 后景色
	/// @param brush 変えたいBrush
	void SetBrushColor(D2D1::ColorF frontColor, D2D1::ColorF backColor,BrushKind brush);

	void UnInit();


private:

	void InitSolidBrush();
	void InitLinearBrush();
	void InitRadianBrush();
};


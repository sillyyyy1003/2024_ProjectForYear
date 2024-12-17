#pragma once
#include <d2d1.h>
#include <map>
#include <string>
#include <variant>
#include <wrl/client.h>
#include "D2DFont.h"
using namespace Microsoft::WRL;

namespace D2DBrushConfig
{
	enum class BrushKind :uint8_t
	{
		SOLID,
		LINEAR,		//linear gradient
		RADIAN		//Radian gradient
	};
	using BrushList = std::map<BrushKind, ID2D1Brush*>;
}
/// @brief D2D Render—pBrush
class D2DBrush
{
private:

	ComPtr<ID2D1SolidColorBrush> mSolidBrush;		//Mono tone
	ComPtr<ID2D1RadialGradientBrush> mRGBrush;		//Radian
	ComPtr<ID2D1LinearGradientBrush> mLGBrush;		//Linear



	D2D1::ColorF mFrontColorF = D2D1::ColorF::White;		//‘OŒiF
	D2D1::ColorF mBackColorF = D2D1::ColorF::Black;			//@ŒiF

	ComPtr<ID2D1GradientStopCollection> mGradientStopLiner = nullptr;
	ComPtr<ID2D1GradientStopCollection> mGradientStopRadian= nullptr;
	using BrushVariant = std::variant<ID2D1SolidColorBrush*, ID2D1LinearGradientBrush*, ID2D1RadialGradientBrush*>;


	D2DBrush() {};
	~D2DBrush() = default;

	D2DBrushConfig::BrushList mBrushList;

public:

	static D2DBrush* Get() { static D2DBrush instance; return &instance; };

	ID2D1SolidColorBrush* GetSolidBrush() { return mSolidBrush.Get(); };		//Mono tone
	ID2D1RadialGradientBrush* GetRGBrush() { return mRGBrush.Get(); };		//Radian
	ID2D1LinearGradientBrush* GetLGBrush() { return mLGBrush.Get(); };

	/// @brief Brush‰Šú‰»
	void InitBrush();

	/// @brief SolidBrush Color‚ğİ’è‚·‚é
	///	@param color İ’è‚·‚éF
	void SetSolidBrushColor(D2D1::ColorF color);

	/// @brief Radian& Linear Brush‚ÌF‚ğİ’è‚·‚é
	/// @param frontColor ‘OŒiF
	/// @param backColor @ŒiF
	/// @param brush •Ï‚¦‚½‚¢Brush
	void SetBrushColor(D2D1::ColorF frontColor, D2D1::ColorF backColor, D2DBrushConfig::BrushKind brush);

	void UnInit();


	const D2DBrushConfig::BrushList& GetBrushList() { return mBrushList; };

private:

	void InitSolidBrush();
	void InitLinearBrush();
	void InitRadianBrush();
};


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
/// @brief D2D Render�pBrush
class D2DBrush
{
private:

	ComPtr<ID2D1SolidColorBrush> mSolidBrush;		//Mono tone
	ComPtr<ID2D1RadialGradientBrush> mRGBrush;		//Radian
	ComPtr<ID2D1LinearGradientBrush> mLGBrush;		//Linear



	D2D1::ColorF mFrontColorF = D2D1::ColorF::White;		//�O�i�F
	D2D1::ColorF mBackColorF = D2D1::ColorF::Black;			//�@�i�F

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

	/// @brief Brush������
	void InitBrush();

	/// @brief SolidBrush Color��ݒ肷��
	///	@param color �ݒ肷��F
	void SetSolidBrushColor(D2D1::ColorF color);

	/// @brief Radian& Linear Brush�̐F��ݒ肷��
	/// @param frontColor �O�i�F
	/// @param backColor �@�i�F
	/// @param brush �ς�����Brush
	void SetBrushColor(D2D1::ColorF frontColor, D2D1::ColorF backColor, D2DBrushConfig::BrushKind brush);

	void UnInit();


	const D2DBrushConfig::BrushList& GetBrushList() { return mBrushList; };

private:

	void InitSolidBrush();
	void InitLinearBrush();
	void InitRadianBrush();
};


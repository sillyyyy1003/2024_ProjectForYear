#pragma once
#include <d2d1.h>
#include <DirectXCollision.h>
#include <map>
#include <string>
#include <wrl/client.h>
#include "D3DApp.h"
#include "UI_Font.h"
using namespace Microsoft::WRL;

struct ID2D1SolidColorBrush;
struct ID2D1RadialGradientBrush;
struct ID2D1LinearGradientBrush;

namespace D2DUIConfig
{

	enum class FontSize :uint8_t
	{
		TITLE_SIZE,
		NORMAL_SIZE,
		SMALL_SIZE,
	};

	constexpr float FONT_TITLE_SIZE = 180.f;
	constexpr float FONT_NORMAL_SIZE = 80.f;
	constexpr float FONT_SMALL_SIZE = 24.f;

	enum class TextAlignment:uint8_t
	{
		LEFT,		//LEADING
		CENTER,		//TRAILING
		RIGHT,		//CENTER
		JUSTIFIED	//JUSTIFIED
	};


}


///@brief •¶Žš‚Ì•`‰æ
class D2DFont
{
private:

	//Brushes
	ComPtr<ID2D1SolidColorBrush> mSolidBrush;		//Mono tone
	ComPtr<ID2D1RadialGradientBrush> mRadianGBrush;		//Radian
	ComPtr<ID2D1LinearGradientBrush> mLinearGBrush;		//Linear

	//Text Format
	ComPtr<IDWriteTextFormat> mJpnTitleSizeFormat = nullptr;
	ComPtr<IDWriteTextFormat> mJpnNormalSizeFormat = nullptr;
	ComPtr<IDWriteTextFormat> mJpnSmallSizeFormat = nullptr;

	//Color
	//Radian
	D2D1::ColorF mRadianFrontColor = D2D1::ColorF::White;
	D2D1::ColorF mRadianBackColor = D2D1::ColorF::Black;
	//Linear
	D2D1::ColorF mLinearFrontColor = D2D1::ColorF::White;
	D2D1::ColorF mLinearBackColor = D2D1::ColorF::Black;
	//Solid
	D2D1::ColorF mSolidColor = D2D1::ColorF::White;

	ComPtr<ID2D1GradientStopCollection> mGradientStopLiner = nullptr;
	ComPtr<ID2D1GradientStopCollection> mGradientStopRadian = nullptr;

	D2DFont(){};
	~D2DFont(){}

public:
	static D2DFont* Get() { static D2DFont instance; return &instance; };

	void InitD2DFont();

	void SetSolidBrushColor(D2D1::ColorF color);
	void DrawSolidFont(D2DUIConfig::FontSize size, const char* text, D2D1_RECT_F  rect);

	void DrawSolidFont(D2DUIConfig::FontSize size, const char* text,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 scale);


	/// @brief Draw Solid Font 
	/// @param size Solid Font size
	/// @param color font color
	/// @param text text contents
	/// @param pos pos
	/// @param scale size
	void DrawSolidText(D2DUIConfig::FontSize size, D2D1::ColorF color, const char* text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale);

	void SetTextAlign(D2DUIConfig::FontSize font, D2DUIConfig::TextAlignment alignment);

	void SetTextFormatAlignment(IDWriteTextFormat* format,D2DUIConfig::TextAlignment alignment);

	/// @brief Fill the RoundedRect
	/// @param roundRect 
	void DrawRoundRect(D2D1_ROUNDED_RECT roundRect);

	/// @brief Draw the rounded rect
	/// @param roundRect 
	void DrawRoundRectOutline(D2D1_ROUNDED_RECT roundRect);


	




};


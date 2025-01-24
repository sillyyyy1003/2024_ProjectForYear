#include "D2DFont.h"

void D2DFont::InitD2DFont()
{
	//Init Brush
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	IDWriteFactory* writeFactory = gD3D->GetWriteFactory();

	//Create Solid Brush
	renderTarget->CreateSolidColorBrush(mSolidColor, mSolidBrush.GetAddressOf());

	//Create Linear Gradient Brush
	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0] = { 0.0f, mLinearFrontColor };
	gradientStops[1] = { 1.0f,  mLinearBackColor };

	renderTarget->CreateGradientStopCollection(gradientStops, 2, mGradientStopLiner.GetAddressOf());
	renderTarget->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0),
			D2D1::Point2F(100, 100)),
		mGradientStopLiner.Get(),
		mLinearGBrush.GetAddressOf());

	//Create Radian Gradient Brush
	gradientStops[0] = { 0.0f, mRadianFrontColor };
	gradientStops[1] = { 1.0f,  mRadianBackColor };

	renderTarget->CreateGradientStopCollection(gradientStops, 2, mGradientStopRadian.GetAddressOf());
	renderTarget->CreateRadialGradientBrush(
		D2D1::RadialGradientBrushProperties(
			D2D1::Point2F(50, 50),
			D2D1::Point2F(0, 0),
			50, 50),
		mGradientStopRadian.Get(),
		mRadianGBrush.GetAddressOf());


	//Init Text Format
	HR(writeFactory->CreateTextFormat(
		L"Hangyaku",    // Font family name
		nullptr,        // Font collection (nullptr for system default)
		DWRITE_FONT_WEIGHT_NORMAL,	// Font weight (e.g., Normal, Bold)
		DWRITE_FONT_STYLE_NORMAL,   // Font style (e.g., Normal, Italic)
		DWRITE_FONT_STRETCH_EXPANDED, // Font stretch (e.g., Normal, Condensed)
		D2DUIConfig::FONT_TITLE_SIZE,                         // Font size
		L"ja-JP",                   // Locale (e.g., en-us for English, zh-cn for Simplified Chinese)
		mJpnTitleSizeFormat.GetAddressOf()// Address of the text format pointer
	));

	HR(writeFactory->CreateTextFormat(
		L"Hangyaku",    // Font family name
		nullptr,        // Font collection (nullptr for system default)
		DWRITE_FONT_WEIGHT_NORMAL,	// Font weight (e.g., Normal, Bold)
		DWRITE_FONT_STYLE_NORMAL,   // Font style (e.g., Normal, Italic)
		DWRITE_FONT_STRETCH_EXPANDED, // Font stretch (e.g., Normal, Condensed)
		D2DUIConfig::FONT_NORMAL_SIZE,                         // Font size
		L"ja-JP",                   // Locale (e.g., en-us for English, zh-cn for Simplified Chinese)
		mJpnNormalSizeFormat.GetAddressOf()// Address of the text format pointer
	));

	HR(writeFactory->CreateTextFormat(
		L"Hangyaku",    // Font family name
		nullptr,        // Font collection (nullptr for system default)
		DWRITE_FONT_WEIGHT_NORMAL,	// Font weight (e.g., Normal, Bold)
		DWRITE_FONT_STYLE_NORMAL,   // Font style (e.g., Normal, Italic)
		DWRITE_FONT_STRETCH_EXPANDED, // Font stretch (e.g., Normal, Condensed)
		D2DUIConfig::FONT_SEMI_SIZE,                         // Font size
		L"ja-JP",                   // Locale (e.g., en-us for English, zh-cn for Simplified Chinese)
		mJpnSemiSizeFormat.GetAddressOf()// Address of the text format pointer
	));


	HR(writeFactory->CreateTextFormat(
		L"Hangyaku",    // Font family name
		nullptr,        // Font collection (nullptr for system default)
		DWRITE_FONT_WEIGHT_NORMAL,	// Font weight (e.g., Normal, Bold)
		DWRITE_FONT_STYLE_NORMAL,   // Font style (e.g., Normal, Italic)
		DWRITE_FONT_STRETCH_NORMAL, // Font stretch (e.g., Normal, Condensed)
		D2DUIConfig::FONT_SMALL_SIZE,                         // Font size
		L"ja-JP",                   // Locale (e.g., en-us for English, zh-cn for Simplified Chinese)
		mJpnSmallSizeFormat.GetAddressOf()// Address of the text format pointer
	));

	
}

void D2DFont::SetSolidBrushColor(D2D1::ColorF color)
{
	mSolidBrush->SetColor(color);
}

void D2DFont::DrawSolidFont(D2DUIConfig::FontSize size, const char* text, D2D1_RECT_F rect)
{
	std::string str(text);
	int strSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wStr(strSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wStr[0], strSize);

	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();
	//D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(D2D1::SizeF(2.0f, 2.0f), D2D1::Point2F(50.0f, 50.0f));
	//renderTarget->SetTransform(scaleMatrix);
	switch(size)
	{
		default:
	case D2DUIConfig::FontSize::TITLE_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF color = D2D1::ColorF::Red;
		mSolidBrush->SetColor(color);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(), mJpnTitleSizeFormat.Get(), rect, mSolidBrush.Get());
	}
		break;
	case D2DUIConfig::FontSize::NORMAL_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF color = D2D1::ColorF::Red;
		mSolidBrush->SetColor(color);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(), mJpnNormalSizeFormat.Get(), rect, mSolidBrush.Get());
	}
		break;
	case D2DUIConfig::FontSize::SEMI_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF color = D2D1::ColorF::Red;
		mSolidBrush->SetColor(color);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(), mJpnSemiSizeFormat.Get(), rect, mSolidBrush.Get());
	}
	break;
	case D2DUIConfig::FontSize::SMALL_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF color = D2D1::ColorF::Red;
		mSolidBrush->SetColor(color);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(), mJpnSmallSizeFormat.Get(), rect, mSolidBrush.Get());
	}
		break;
	}

	renderTarget->EndDraw();
}

void D2DFont::DrawSolidFont(D2DUIConfig::FontSize size, const char* text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale)
{
	//transform rect to screen
	float centerX = pos.x + WIN_WIDTH / 2.f;
	float centerY = WIN_HEIGHT / 2.f - pos.y;
	D2D1_RECT_F rect;
	rect.left = centerX - scale.x / 2.f;
	rect.top= centerY - scale.y / 2.f;
	rect.right= centerX + scale.x / 2.f;
	rect.bottom= centerY + scale.y / 2.f;
	DrawSolidFont(size, text, rect);
}

void D2DFont::DrawSolidText(D2DUIConfig::FontSize size, D2D1::ColorF color, const char* text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale)
{

	std::string str(text);
	int strSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wStr(strSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wStr[0], strSize);

	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();
	

	float centerX = pos.x + WIN_WIDTH / 2.f;
	float centerY = WIN_HEIGHT / 2.f - pos.y;
	D2D1_RECT_F rect;
	rect.left = centerX - scale.x / 2.f;
	rect.top = centerY - scale.y / 2.f;
	rect.right = centerX + scale.x / 2.f;
	rect.bottom = centerY + scale.y / 2.f;

	
	switch (size)
	{
	case D2DUIConfig::FontSize::TITLE_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF debugColor = D2D1::ColorF::Red;
		mSolidBrush->SetColor(debugColor);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		mSolidBrush->SetColor(color);
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(), mJpnTitleSizeFormat.Get(), rect, mSolidBrush.Get());
	}
	break;
	case D2DUIConfig::FontSize::NORMAL_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF debugColor = D2D1::ColorF::Red;
		mSolidBrush->SetColor(debugColor);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		mSolidBrush->SetColor(color);
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(), mJpnNormalSizeFormat.Get(), rect, mSolidBrush.Get());
	}
	break;
	case D2DUIConfig::FontSize::SEMI_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF debugColor = D2D1::ColorF::Red;
		mSolidBrush->SetColor(debugColor);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		mSolidBrush->SetColor(color);
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(), mJpnSemiSizeFormat.Get(), rect, mSolidBrush.Get());
	}
	break;
	case D2DUIConfig::FontSize::SMALL_SIZE:
	{
#ifdef _DEBUG
		D2D1::ColorF debugColor = D2D1::ColorF::Red;
		mSolidBrush->SetColor(debugColor);
		renderTarget->DrawRectangle(rect, mSolidBrush.Get());
#endif
		mSolidBrush->SetColor(color);
		renderTarget->DrawTextW(wStr.c_str(), (UINT32)wStr.size(),mJpnSmallSizeFormat.Get(), rect, mSolidBrush.Get());
	}
	break;
	}
	
	renderTarget->EndDraw();
}

void D2DFont::SetTextAlign(D2DUIConfig::FontSize font, D2DUIConfig::TextAlignment alignment)
{
	switch (font)
	{
	default:
	case D2DUIConfig::FontSize::TITLE_SIZE:
		SetTextFormatAlignment(mJpnTitleSizeFormat.Get(), alignment);
		break;
	case D2DUIConfig::FontSize::NORMAL_SIZE:
		SetTextFormatAlignment(mJpnNormalSizeFormat.Get(), alignment);
		break;
	case D2DUIConfig::FontSize::SEMI_SIZE:
		SetTextFormatAlignment(mJpnSemiSizeFormat.Get(), alignment);
		break;
	case D2DUIConfig::FontSize::SMALL_SIZE:
		SetTextFormatAlignment(mJpnSmallSizeFormat.Get(), alignment);
		break;
	}
}

void D2DFont::SetTextFormatAlignment(IDWriteTextFormat* format, D2DUIConfig::TextAlignment alignment)
{
	switch(alignment)
	{
	default:
	case D2DUIConfig::TextAlignment::LEFT:
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case D2DUIConfig::TextAlignment::CENTER:
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case D2DUIConfig::TextAlignment::RIGHT:
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	case D2DUIConfig::TextAlignment::JUSTIFIED:
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		break;

	}
}

void D2DFont::SetFontScale(DirectX::XMFLOAT2 _size)
{
	mFontScale = _size;
}


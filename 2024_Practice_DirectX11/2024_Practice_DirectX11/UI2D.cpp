#include "UI2D.h"
#include "D3DApp.h"
#include <string>
#include <locale>
#include <codecvt>

UI2D::UI2D()
{
}

UI2D::~UI2D()
{
}

void UI2D::InitUI2D()
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	IDXGISwapChain* swapChain = gD3D->GetSwapChain();
	ID2D1Factory* D2DFactory = gD3D->GetD2DFactory();
	IDWriteFactory* writeFactory = gD3D->GetWriteFactory();


	//Brush Init
	mBrush = std::make_unique<D2DBrush>();
	mBrush->InitBrush();

	//TextFormat Init
	HR(writeFactory->CreateTextFormat(
			L"HG創英角ﾎﾟｯﾌﾟ体",    // Font family name
			nullptr,        // Font collection (nullptr for system default)
			DWRITE_FONT_WEIGHT_NORMAL,	// Font weight (e.g., Normal, Bold)
			DWRITE_FONT_STYLE_NORMAL,   // Font style (e.g., Normal, Italic)
			DWRITE_FONT_STRETCH_NORMAL, // Font stretch (e.g., Normal, Condensed)
			30,                         // Font size
			L"ja-JP",                   // Locale (e.g., en-us for English, zh-cn for Simplified Chinese)
			mTextFormat.GetAddressOf()// Address of the text format pointer
		));
}

void UI2D::Draw()
{
	
}

void UI2D::DrawTextC(const char* str, BrushKind brush)
{
	std::string string(str);
	DrawTextStr(string,brush);
}

void UI2D::DrawTextStr(std::string str, BrushKind brush)
{
	
	//Convert string to wide string
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);

	DrawTextWStr(wstr, brush);
}

void UI2D::DrawTextWStr(std::wstring wstr, BrushKind brush)
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();
	switch (brush)
	{
	case SOLID:
		renderTarget->DrawTextW(wstr.c_str(), (UINT32)wstr.size(), mTextFormat.Get(),
			D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mBrush->mSolidBrush.Get());
		break;

	case LINEAR:
		renderTarget->DrawTextW(wstr.c_str(), (UINT32)wstr.size(), mTextFormat.Get(),
			D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mBrush->mLGBrush.Get());
		break;

	case RADIAN:
		renderTarget->DrawTextW(wstr.c_str(), (UINT32)wstr.size(), mTextFormat.Get(),
			D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mBrush->mRGBrush.Get());
		break;

	}
	renderTarget->EndDraw();
}


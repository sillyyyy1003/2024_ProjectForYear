#include "UI2D.h"
#include "D3DApp.h"
#include <string>
#include <locale>
#include <codecvt>
#include "GampApp.h"

UI2D::UI2D()
{
}

UI2D::~UI2D()
{
}

void UI2D::Update()
{
	if (GameApp::Get()->GetResized())
	{
		float factorY = (float)GameApp::Get()->GetWinHeight() / WIN_HEIGHT;
		float factorX = (float)GameApp::Get()->GetWinWidth() / WIN_WIDTH;

		float newWidth = mWidth * factorX;
		float newHeight = mHeight * factorY;

		float centerX = mCenterX * factorX;
		float centerY = mCenterY * factorY;
		SetRect(centerX, centerY, newWidth, newHeight);
	}
}

void UI2D::InitUI2D(float centerX, float centerY, float width, float height)
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	IDXGISwapChain* swapChain = gD3D->GetSwapChain();
	ID2D1Factory* D2DFactory = gD3D->GetD2DFactory();
	IDWriteFactory* writeFactory = gD3D->GetWriteFactory();

	mCenterX = centerX;
	mCenterY = centerY;
	mWidth = width;
	mHeight = height;

	//InitRect
	mDrawArea.left = mCenterX - (mWidth / 2);
	mDrawArea.right = mCenterX + (mWidth / 2);
	mDrawArea.top = mCenterY - (mHeight / 2);
	mDrawArea.bottom = mCenterY + (mHeight / 2);

	//Brush Init
	mBrush = std::make_unique<D2DBrush>();
	mBrush->InitBrush();

	//TextFormat Init
	//todo:読み込みができるようにする
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

void UI2D::DrawTextC(const char* str, BrushKind brush)
{
	std::string string(str);
	DrawTextStr(string,brush);
}

void UI2D::DrawTextStr(std::string str, BrushKind brush)
{
	//Convert string to wide string
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);

	DrawTextWStr(wstr, brush);


}

void UI2D::DrawTextWStr(std::wstring wstr, BrushKind brush)
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();
	renderTarget->DrawTextW(wstr.c_str(), (UINT32)wstr.size(), mTextFormat.Get(), mDrawArea, mBrush->mBrushList.at(brush));
	renderTarget->EndDraw();
}

void UI2D::SetRect(float centerX, float centerY, float width, float height)
{
	mDrawArea.left = centerX - (width / 2);
	mDrawArea.right = centerX + (width / 2);
	mDrawArea.top = centerY - (height / 2);
	mDrawArea.bottom = centerY + (height / 2);
}

void UI2D::SetCenter(float _centerX, float _centerY)
{
	mCenterX = _centerX;
	mCenterY = _centerY;
	mDrawArea.left = mCenterX - (mWidth / 2);
	mDrawArea.right = mCenterX + (mWidth / 2);
	mDrawArea.top = mCenterY - (mHeight / 2);
	mDrawArea.bottom = mCenterY + (mHeight / 2);

}

void UI2D::SetWidth(float _width)
{
	mWidth = _width;
	mDrawArea.left = mCenterX - (mWidth / 2);
	mDrawArea.right = mCenterX + (mWidth / 2);
	mDrawArea.top = mCenterY - (mHeight / 2);
	mDrawArea.bottom = mCenterY + (mHeight / 2);
}

void UI2D::SetHeight(float _height)
{
	mHeight = _height;
	mDrawArea.left = mCenterX - (mWidth / 2);
	mDrawArea.right = mCenterX + (mWidth / 2);
	mDrawArea.top = mCenterY - (mHeight / 2);
	mDrawArea.bottom = mCenterY + (mHeight / 2);
}

void UI2D::DrawRect(BrushKind _rectBrush)
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();
	renderTarget->FillRectangle(&mDrawArea, mBrush->mBrushList.at(_rectBrush));
	renderTarget->EndDraw();
}

void UI2D::DrawSolidInfoBox(std::string str, D2D1::ColorF _textColor, D2D1::ColorF _boxColor)
{
	ID2D1RenderTarget* renderTarget = gD3D->Get2DRenderTarget();
	renderTarget->BeginDraw();

	mBrush->SetSolidBrushColor(_boxColor);
	renderTarget->FillRectangle(&mDrawArea, mBrush->mBrushList.at(SOLID));

	mBrush->SetSolidBrushColor(_textColor);
	//文字変換
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
	renderTarget->DrawTextW(wstr.c_str(), (UINT32)wstr.size(), mTextFormat.Get(), mDrawArea, mBrush->mBrushList.at(SOLID));

	renderTarget->EndDraw();
}

void UI2D::SetNewFontSize(float _size)
{
	mTextFormat.Reset();
	IDWriteFactory* writeFactory = gD3D->GetWriteFactory();
	HRESULT hr;
	hr= writeFactory->CreateTextFormat(
		L"HG創英角ﾎﾟｯﾌﾟ体",    // Font family name
		nullptr,        // Font collection (nullptr for system default)
		DWRITE_FONT_WEIGHT_NORMAL,	// Font weight (e.g., Normal, Bold)
		DWRITE_FONT_STYLE_NORMAL,   // Font style (e.g., Normal, Italic)
		DWRITE_FONT_STRETCH_NORMAL, // Font stretch (e.g., Normal, Condensed)
		30,                         // Font size
		L"ja-JP",                   // Locale (e.g., en-us for English, zh-cn for Simplified Chinese)
		mTextFormat.GetAddressOf()// Address of the text format pointer
	);

	if(FAILED(hr))
	{
		DebugLog::LogError("Set new font size failed");
	}
}

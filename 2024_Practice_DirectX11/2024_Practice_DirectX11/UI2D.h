#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <memory>
#include <string>
#include <wrl/client.h>
#include "D2DBrush.h"

struct IDWriteFont;
using namespace Microsoft::WRL;
/// <summary>
/// Using D2D Render
/// </summary>
class UI2D
{
private:
	
	ComPtr<IDWriteTextFormat> mTextFormat=nullptr;		// TextFormat
	std::unique_ptr<D2DBrush> mBrush = nullptr;

	//todo:追加Font读取功能
	//ComPtr<IDWriteFont> mFont=nullptr;					// Font

	D2D1_RECT_F mDrawArea = {};
public:
	UI2D();
	~UI2D();

	/// <summary>
	/// UIの初期化
	/// </summary>
	void InitUI2D();

	/// @brief 文字を表示する
	/// @param str 文字内容 char*
	/// @param brush 使うBrush
	void DrawTextC(const char* str, BrushKind brush);

	/// @brief 文字を表示する
	/// @param str 文字内容 std::string
	/// @param brush 使うBrush
	void DrawTextStr(std::string str, BrushKind brush);

	/// @brief 文字を表示する
	/// @param wstr 文字内容 std::wstring
	/// @param brush 使うBrush
	void DrawTextWStr(std::wstring wstr, BrushKind brush);

	/// @brief 描画のエリアを設定
	/// @param left 
	/// @param top 
	/// @param right 
	/// @param bottom 
	void SetRect(float left, float top, float right, float bottom);


};


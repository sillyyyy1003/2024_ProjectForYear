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
	
	std::unique_ptr<D2DBrush> mBrush = nullptr;			// Brush
	ComPtr<IDWriteTextFormat> mTextFormat=nullptr;		// TextFormat

	//todo:追加Font读取功能
	//ComPtr<IDWriteFont> mFont=nullptr;					// Font


public:
	UI2D();
	~UI2D();

	/// <summary>
	/// UIの初期化
	/// </summary>
	void InitUI2D();



	void Draw();

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


};


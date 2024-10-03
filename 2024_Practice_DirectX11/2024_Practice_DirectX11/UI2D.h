#pragma once
#include <d2d1.h>
#include <DirectXCollision.h>
#include <dwrite.h>
#include <memory>
#include <string>
#include <wrl/client.h>
#include "D2DBrush.h"
#include "SceneBase.h"

struct IDWriteFont;
using namespace Microsoft::WRL;
/// <summary>
/// Using D2D Render
/// </summary>
class UI2D :public Component
{
private:

	ComPtr<IDWriteTextFormat> mTextFormat = nullptr;		// TextFormat
	std::unique_ptr<D2DBrush> mBrush = nullptr;

	//todo:追加Font读取功能
	//ComPtr<IDWriteFont> mFont=nullptr;				// Font

	D2D1_RECT_F mDrawArea = {};
	float mCenterX=0, mCenterY = 0;
	float mWidth=0, mHeight = 0;


public:
	UI2D();
	~UI2D();

	void Update();
	D2DBrush* GetBrush() { return mBrush.get(); };

	/// @brief 初期化
	/// @param centerX 中心点横位置 
	/// @param centerY 中心点縦位置
	/// @param width 幅
	/// @param height 高さ
	void InitUI2D(float centerX, float centerY, float width, float height);


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
	/// @param centerX 中心点横位置 
	/// @param centerY 中心点縦位置
	/// @param width 幅
	/// @param height 高さ
	void SetRect(float centerX, float centerY, float width, float height);

	/// @brief Rectの中心点
	/// @param _centerX 
	/// @param _centerY 
	void SetCenter(float _centerX, float _centerY);

	void SetWidth(float _width);

	void SetHeight(float _height);

	/// @brief 四角形を描画する
	///	@param _rectBrush 使うBrush
	void DrawRect(BrushKind _rectBrush);

	/// @brief  メッセージが含まれるTalk Box(背景 文字　SOLID)
	/// @param str str 文字内容 string
	/// @param _textColor 文字 色
	/// @param _boxColor 背景色
	void DrawSolidInfoBox(std::string str ,D2D1::ColorF _textColor,D2D1::ColorF _boxColor);

	/// @brief Font Size設定
	/// @param _size 
	void SetNewFontSize(float _size);

};


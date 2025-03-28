﻿#pragma once
#include "CameraBase.h"
#include "Square.h"
#include "UI_Square.h"

namespace UITextOption
{
	/// @brief Text Alignment
	enum class TextAlign
	{
		Left = 0,
		Center = 1,
		Right = 2 ,
	};

	/// @brief Text Anchor Pos
	enum class AnchorAlign
	{
		TopLeft = 0,
		TopCenter = 1,
		TopRight = 2,
		CenterLeft = 3,
		Center = 4,
		CenterRight = 5,
		BottomLeft = 6,
		BottomCenter = 7,
		BottomRight = 8

	};


	inline const char* TextAlignToString(TextAlign align)
	{
		switch (align)
		{
		case TextAlign::Left: 
			return "Left";
		case TextAlign::Center: 
			return "Center";
		case TextAlign::Right: 
			return "Right";
		default: 
			return "Unknown";
		}
	}

	inline const char* AnchorAlignToString(AnchorAlign align)
	{
		switch (align)
		{
		case AnchorAlign::TopLeft: 
			return "Top Left";
		case AnchorAlign::TopCenter: 
			return "Top Center";
		case AnchorAlign::TopRight: 
			return "Top Right";
		case AnchorAlign::CenterLeft: 
			return "Center Left";
		case AnchorAlign::Center: 
			return "Center";
		case AnchorAlign::CenterRight: 
			return "Center Right";
		case AnchorAlign::BottomLeft: 
			return "Bottom Left";
		case AnchorAlign::BottomCenter: 
			return "Bottom Center";
		case AnchorAlign::BottomRight: 
			return "Bottom Right";
		default: 
			return "Unknown";
		}
	}

	constexpr int defaultMaxChar = 256;
	constexpr DirectX::XMINT2 defaultSplit = { 12,8 };

	constexpr float defaultFontRectWidth = 300.0f;

	constexpr DirectX::XMFLOAT2 FONT_DEFAULT_SIZE = { 2.f,3.36f };//1:1.68
	constexpr float defaultFontLayerPosZ = 0.2f;
	
}

///文字表示だけ
///todo:Change this to a Large ConstantBuffer
class UI_Font :public Component
{
private:

	float startX, startY, endX, endY = 0;
protected:

	std::vector<std::shared_ptr<UI_Square>> mText;	//描画する文字列

	MaterialData mTexData;	//すべての文字が使う文字テクスチャ

	DirectX::XMFLOAT2 mAnchorPos = { 0,0 };	// Anchor Position
	DirectX::XMFLOAT2 mCharSize = { 1,1 };	// Char Size

	/// @brief 
	/// 等比率scale up/down
	///	Use char width as unit
	float mFontSize = 1.0f;		
	float mLineSpacing = 0.0f;	//行間 Use Pixel as unit

	bool isSetWidth = false;
	float mBlockWidth = UITextOption::defaultFontRectWidth;		//文字ボロックの幅

	std::string mContent;	//表示する文字列

	UITextOption::TextAlign mTextAlign = UITextOption::TextAlign::Left;			//Default is Left
	UITextOption::AnchorAlign mAnchorAlign = UITextOption::AnchorAlign::TopLeft;//Default is left top

	float posZ = UITextOption::defaultFontLayerPosZ;

#ifdef _DEBUG
	//Debug用表示
	std::unique_ptr<UI_Square> mDebugAnchorPos = nullptr;
	std::unique_ptr<UI_Square> mDebugRect = nullptr;
	bool isShowDebugRect = false;
#endif

	/// @brief 
	bool isFontSizeChanged = false;
	

public:

	UI_Font();
	~UI_Font() = default;

	/// @brief Font Libを読み込み
	/// @param filePath File path
	///	@param charSize font サイズを設定する
	void Init(const char* filePath, DirectX::XMFLOAT2 charSize);

	/// @brief Font Libを読み込み
	/// @param fontTex 既にロードされたTex
	///	@param charSize font サイズを設定する
	void Init(const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 charSize);

	void SetFont(std::shared_ptr<Texture> fontTex);

	/// @brief Set text block Width
	/// @param width 
	void SetFontRectWidth(float width);

	float GetFontRectWidth() { return mBlockWidth; };

	/// @brief Reset the trigger
	/// @param isSetWidth 
	void ResetFontWidth(bool isSetWidth) { this->isSetWidth = isSetWidth; };

	void Update();

	/// @brief 文字列の描画
	void Draw();

	/// @brief 描画する文字列を設定する
	/// @param str 
	void SetContent(const char* str);

	/// @brief Anchorの位置揃えを設定する
	/// @param align 
	void SetAnchorAlign(UITextOption::AnchorAlign align);

	/// @brief 文字揃えの設定
	/// @param align 
	void SetTextAlign(UITextOption::TextAlign align);

	/// @brief CharSize変更する
	void SetCharSize(DirectX::XMFLOAT2 _size)noexcept;

	/// @brief FontSizeを変更（等比率）
	/// @param _fontSize 
	void SetFontSize(float _fontSize)noexcept { mFontSize = _fontSize; };

	void SetLineSpacing(float _lineSpacing)noexcept { mLineSpacing = _lineSpacing; };

	/// @brief フォントのサイズをWindowによって変換する
	void UpdateCharSize() noexcept;

	/// @brief UIのAnchor位置を設定する
	void SetAnchorPos(float x, float y);
	void SetAnchorPos(DirectX::XMFLOAT2 anchorPos);

	/// @brief 表示する文字の位置を調整する
	void UpdatePosition();

	void SetFontColor(DirectX::XMFLOAT4 color);
	const DirectX::XMFLOAT4& GetFontColor() { return mText[0]->GetDiffuseColor(); };

	void SetPosZ(float posZ) { this->posZ = posZ; };
	UITextOption::TextAlign GetTextAlign()const { return mTextAlign; };
	UITextOption::AnchorAlign GetAnchorAlign() const { return mAnchorAlign; };
	DirectX::XMFLOAT3 GetAnchorPos() const { return DirectX::XMFLOAT3{ mAnchorPos.x,mAnchorPos.y,posZ }; };

#ifdef _DEBUG
	void DebugFunction();
#endif
	std::string GetContext() const { return mContent; };

	float GetFontSize()const { return mFontSize; };
	float GetLineSpacing()const { return mLineSpacing; };


	void InitDebugFunction();
	void NotifyFontSizeChanged() { isFontSizeChanged = true; };
	void ClearFontSizeChanged() { isFontSizeChanged = false; };
	bool GetFontSizeChanged() { return isFontSizeChanged; };
};


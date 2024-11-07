#pragma once
#include "CameraBase.h"
#include "CanvasUI.h"
#include "Square.h"

namespace UITextOption
{
	/// @brief Text Alignment
	enum class TextAlign
	{
		Left = 0 ,
		Center,
		Right,
	};

	/// @brief Text Anchor Pos
	enum class AnchorAlign
	{
		TopLeft = 0,
		TopCenter,   
		TopRight,    
		CenterLeft,  
		Center,      
		CenterRight, 
		BottomLeft,
		BottomCenter,
		BottomRight

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

	constexpr int defaultMaxChar = 512;
	constexpr DirectX::XMINT2 defaultSplit = { 12,8 };

	constexpr float defaultFontRectWidth = 200.0f;

	constexpr DirectX::XMFLOAT2 fontDefaultSize = { 20.f,45.25f };

	//todo:this part is used to contain all the fontData
	using FontSet = std::map<std::string, DirectX::XMFLOAT2>;//Font file path, Name Font Default Size
	using FontList = std::map<std::string, FontSet>;	//Font Name, Font Set
	
}

///文字表示だけ
class UIFont :public Component
{
private:
	
	DirectX::XMFLOAT2 mOriginalSize = { 1,1 };
	DirectX::XMFLOAT2 mOriginalPos = { 0.0f,0.0f };

protected:

	std::vector<std::shared_ptr<CanvasUI>> mText;	//描画する文字列
	MaterialData mTexData;

	DirectX::XMFLOAT2 mAnchorPos = { 0,0 };	// Anchor Position
	DirectX::XMFLOAT2 mSize = { 1,1 };
	

	bool isSetWidth = false;
	float mFontWidthRectWidth = UITextOption::defaultFontRectWidth;

	std::string mContent;

	UITextOption::TextAlign mTextAlign = UITextOption::TextAlign::Left;			//Default is Left
	UITextOption::AnchorAlign mAnchorAlign = UITextOption::AnchorAlign::TopLeft;//Default is left top

	std::unique_ptr<CanvasUI> mDebugAnchorPos = nullptr;

public:

	UIFont();
	~UIFont() = default;

	/// @brief Font Libを読み込み
	/// @param filePath File path
	///	@param fontSize font サイズを設定する
	void Init(const char* filePath, DirectX::XMFLOAT2 fontSize);

	/// @brief Set text block Width
	/// @param width 
	void SetFontRectWidth(float width);

	/// @brief Reset the trigger
	/// @param isSetWidth 
	void ResetFontWidth(bool isSetWidth) { this->isSetWidth = isSetWidth; };

	void Update();

	/// @brief 文字列の描画
	void Draw();

	void ResetAnchorPos(float x, float y);
	void ResetAnchorPos(DirectX::XMFLOAT2 anchorPos);

	/// @brief 描画する文字列を設定する
	/// @param str 
	void SetContent(const char* str);

	void SetAnchorAlign(UITextOption::AnchorAlign align);

	void SetTextAlign(UITextOption::TextAlign align);

	//Fontの大きさを直接変更する
	void ResetSize(DirectX::XMFLOAT2 _size)noexcept;

	void UpdateChar(const char* str, UITextOption::TextAlign align = UITextOption::TextAlign::Left, UITextOption::AnchorAlign anchor = UITextOption::AnchorAlign::TopLeft);

	/// @brief フォントのサイズをWindowによって変換する
	void UpdateCharSize() noexcept;

	/// @brief 表示する内容を書き換える
	/// @param str 書き換える内容
	void UpdateContents(const char* str) noexcept;

	/// @brief 表示する内容を書き換える
	/// @param str 書き換える内容
	void UpdateContents(std::string str) noexcept;

	/// @brief 表示する文字の位置を調整する
	void UpdatePosition();

	void DebugFunction();


protected:
	//void UpdateChar(const char* str, UITextOption::TextAlign align = UITextOption::TextAlign::Left, UITextOption::AnchorAlign anchor = UITextOption::AnchorAlign::TopLeft);

};


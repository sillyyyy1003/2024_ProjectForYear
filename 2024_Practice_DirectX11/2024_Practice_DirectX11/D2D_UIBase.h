#pragma once
#include "SceneBase.h"
#include "D2DBrush.h"
namespace D2DUIConfig
{
	enum class UIShape
	{
		DEFAULT,
		RECT,				//正方形
		ROUNDED_RECT,		//円角正方形・カプセル
		ELLIPSE,			//楕円
		
	};

	enum UIState :UINT
	{
		STATE_USE_FONT = 1 << 0,
		STATE_USE_BACKGROUND = 1 << 1,
	};

	//constexpr UINT STATE_USE_FONT = 1 << 0;
	//constexpr UINT STATE_USE_BACKGROUND = 1 << 1;


}

///@brief 派生类用于描绘各类UI形状
class D2D_UIBase
{
protected:
	//to be continued
	D2D1::ColorF mBaseColor = { 1,1,1,1 };
	D2D1::ColorF mOutlineColor = { 1,1,1,1 };

	float mRadiusX = 1.f;
	float mRadiusY = 1.f;

	float mOutlineWeight = 1.f;					// Outline width
	DirectX::XMFLOAT2 mPos = { 0,0 };		// Rect Center
	DirectX::XMFLOAT2 mScale = { 1,1 };		// Rect width/height
	D2DUIConfig::UIShape mShape = D2DUIConfig::UIShape::DEFAULT;	// UI Shape
public:
	D2D_UIBase(D2DUIConfig::UIShape shape);
	virtual ~D2D_UIBase() = default;
	virtual void Draw(ID2D1Brush* outlineBrush, ID2D1Brush* brush){};
	virtual void Draw(){};
	virtual void DrawWithRadianBrush(){};

	virtual void SetPosition(DirectX::XMFLOAT2 pos) { mPos = pos; };
	virtual void SetScale(DirectX::XMFLOAT2 scale) { mScale = scale; };

	virtual void SetRadiusX(float radiusX) { mRadiusX = radiusX; };
	virtual void SetRadiusY(float radiusY) { mRadiusY = radiusY; };

	virtual void SetBaseColor(D2D1::ColorF color) { mBaseColor = color; };
	virtual void SetOutlineColor(D2D1::ColorF outlineColor) { mOutlineColor = outlineColor; };
	virtual void SetOutlineWeight(float weight) { mOutlineWeight = weight; };

	virtual json SaveData();
	virtual void LoadSaveData(json data);
	virtual void LoadSaveData(json data, const char* objName);

	DirectX::XMFLOAT4 GetBackGroundColor();
	DirectX::XMFLOAT4 GetOutlineColor();
	float GetOutlineWeight() { return mOutlineWeight; };

	float GetRadiusX() { return mRadiusX; };
	float GetRadiusY() { return mRadiusY; };
};


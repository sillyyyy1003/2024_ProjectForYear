#pragma once
#include "D2D_UIBase.h"



class D2D_UIStackContainer :public Component
{
protected:
#ifdef _DEBUG
	char mInputText[UITextOption::defaultMaxChar] = "";
#endif

	std::unique_ptr<D2D_UIBase> mBackGround;
	std::string mText;

	D2D1::ColorF mFontColor = { 1,1,1,1 };
	D2DUIConfig::FontSize mFontSize;
	D2DUIConfig::TextAlignment mTextAlignment;

	DirectX::XMFLOAT2 mPosition = { 0,0 };
	DirectX::XMFLOAT2 mScale = { 1,1 };
	DirectX::XMFLOAT4 mPadding = {};

	std::string mObjName;

	//Ç∑Ç◊ÇƒÇÃèÛë‘ÇéiÇÈ
	UINT mUiState = 0;
	

public:
	D2D_UIStackContainer();
	virtual ~D2D_UIStackContainer();
	virtual void Init(D2DUIConfig::UIShape shape,D2DUIConfig::FontSize size,const char* objName);
	
	virtual void Update(float dt);
	virtual void Draw();
	virtual void DrawWithRadianBrush();

	virtual void SetText(const char* text);
	virtual void SetFontColor(D2D1::ColorF color);

	virtual json SaveData();
	virtual void LoadSaveData(json data);

#ifdef _DEBUG
	void DebugFunction();
#endif

	void SetUIState(UINT state);
	void RemoveUIState(UINT state);
	void EnableAllState();

	DirectX::XMFLOAT2 GetPosition() { return mPosition; };
	void SetPositionY(float posY) { mPosition.y = posY; };
};


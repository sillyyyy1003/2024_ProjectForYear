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
	DirectX::XMFLOAT2 mFontScale = { 1,1 };

	
	D2DUIConfig::TextAlignment mTextAlignment;

	DirectX::XMFLOAT2 mPosition = { 0,0 };
	DirectX::XMFLOAT2 mScale = { 1,1 };
	DirectX::XMFLOAT4 mPadding = {};

	std::string mObjName;

	//すべての状態を司る
	UINT mUiState = 0;

	float mAccumulateTime = 0.0f;
	float mDuration = 0.0f;

	bool isEmerging = false;	//浮かび上がるスイッチ
	bool isActive = true;
	

public:
	D2D_UIStackContainer();
	virtual ~D2D_UIStackContainer();
	virtual void Init(D2DUIConfig::UIShape shape,D2DUIConfig::FontSize size,const char* objName);
	virtual void Init(D2DUIConfig::UIShape shape,const char* objName);
	
	virtual void Update(float dt);
	virtual void Draw();
	virtual void DrawWithRadianBrush();

	virtual void SetText(const char* text);
	virtual void SetFontColor(D2D1::ColorF color);

	virtual json SaveData();
	virtual void LoadSaveData(json data);
	virtual void SetBackGroundColor(D2D1::ColorF color);

	/// @brief 文字が浮かび上がる機能
	/// @param dt deltaTime
	virtual void EmergingFunction(float dt);
	virtual void InitEmergingFunc(float duration);
	virtual bool GetIsEmerging() const  { return isEmerging; };

	/// @brief 描画のスイッチ
	/// @param isActive 
	/// @return 
	void SetActive(bool isActive) { this->isActive = isActive; };

#ifdef _DEBUG
	void DebugFunction();
#endif

	void SetUIState(D2DUIConfig::UIState state);
	void RemoveUIState(D2DUIConfig::UIState state);
	void EnableAllState();

	DirectX::XMFLOAT2 GetPosition() { return mPosition; };
	void SetPositionY(float posY) { mPosition.y = posY; };
	DirectX::XMFLOAT2 GetScale() { return mScale; };
	
};


#pragma once
#include <memory>
#include "D2D_UIBase.h"
#include "UI_Button.h"



class UI_IconButton : public Component
{
protected:

	//std::unique_ptr<D2D_UIBase> mButtonBackground;	
	std::unique_ptr<UI_Square>	mButtonIcon;	//Icon
	std::unique_ptr<UI_Square>	mBg;			//背景
	std::unique_ptr<UI_Square>	mShadow;		//影

	DirectX::XMFLOAT2 mDefaultPosition = {};	//初期位置
	DirectX::XMFLOAT2 mShadowOffset = {};		//影のずらし

	DirectX::XMFLOAT2 mPosition = {};
	float mPositionOffset = 10.0f;	//y軸のずらし
	DirectX::XMFLOAT2 mScale = {};

	std::string mObjName;
	int mState;

	RECT mRect = {};	//当たり判定

public:
	UI_IconButton(){};
	~UI_IconButton() override = default;

	/// @brief IconButtonの初期化
	/// @param primitiveKind ボタンの形
	///	@param filePath Icon Tex ファイルパス
	///	@param objName オブジェクト名
	void Init(D2DUIConfig::UIShape primitiveKind,const char* filePath,const char* objName);

	/// @brief IconButtonの初期化
	/// @param bgTex 背景テクスチャ
	/// @param iconTex Iconテクスチャ
	/// @param objName オブジェクト名
	virtual void Init(const char* bgTex, const char* iconTex, const char* objName);

	/// @brief ボタンの位置を設定する
	/// @param pos 
	void SetPosition(const DirectX::XMFLOAT2& pos);

	void SetScale(const DirectX::XMFLOAT2& scale);

	bool IsTrigger();

	void Update(float dt);

	virtual void Draw();


	virtual json SaveData();
	virtual void LoadSaveData(json data);

protected:

	virtual void PreUpdate(float dt);
	virtual void GameUpdate(float dt);

	virtual void OnStateNone(float dt);
	virtual void OnStateHover(float dt);
	virtual void OnStatePress(float dt);
	virtual void OnStateTrigger(float dt);

	void UpdateCollider();
};


#pragma once
#include "UIStackContainer.h"
#include "UI_Primitive.h"


namespace UIButtonConfig
{
	enum ButtonState
	{
		STATE_NONE,
		STATE_HOVER,
		STATE_PRESS,
		STATE_MAX,
	};

	constexpr DirectX::XMFLOAT4 DEFAULT_BUTTON_STATE_COLOR[STATE_MAX] = {
		{0.85f, 0.85f, 0.85f, 1.0f},
		{1.f, 1.f, 1.f, 1.0f},
		{1.f, 0.0f, 0.0f, 1.0f},
	};
	
}


class UI_Button :public UIComponent
{
private:
	
	std::unique_ptr<UIStackContainer> mContainer = nullptr;

	//todo: add another primitive to do the outline
	int mState = 0;

	DirectX::XMFLOAT4 mStateColor[UIButtonConfig::STATE_MAX]={};

public:

	UI_Button() = default;
	~UI_Button() override;

	void LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);

	void Init(UIPrimitiveConfig::UI_PrimitiveKind primitiveKind, const std::shared_ptr<Texture>& bgTex, DirectX::XMFLOAT2 containerSize, const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 charSize);

	void Init(UIPrimitiveConfig::UI_PrimitiveKind primitiveKind, const std::shared_ptr<Texture>& bgTex, const std::shared_ptr<Texture>& fontTex);

	void SetStateColor(DirectX::XMFLOAT4 color, UIButtonConfig::ButtonState state);

	void Update() override;
	void Draw(int texSlot = 0) override;

	/// @brief Press 
	/// @return 
	bool isPressed();

	/// @brief Trigger
	/// @return 
	bool isTrigger();

	void LoadSaveData(json data, const char* objName);
	json SaveData(const char* objName);
	json SaveData();

	void SetObjName(const char* objName) { mContainer->SetObjName(objName); };

	void SetEditable(bool isEditable);
protected:
	/// @brief マウスの入力処理を行う
	void PreUpdate();

	/// @brief 入力に応じる処理
	void GameUpdate();

	void SetPosZ(float Z) override;
	void SetPosition(DirectX::XMFLOAT2 pos) { mContainer->SetPosition(pos.x,pos.y); };
	void SetScale(DirectX::XMFLOAT2 scale) { mContainer->SetContainerSize(scale); };

};






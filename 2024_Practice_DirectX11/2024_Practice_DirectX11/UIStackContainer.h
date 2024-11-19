#pragma once
#include "SceneBase.h"
#include "UIFont.h"

namespace UIContainerConfig
{
	enum Padding {
		TOP = 0,
		RIGHT = 1,
		BOTTOM = 2,
		LEFT = 3,
		PADDING_MAX,
	};
}

//todo: make this contain more than one UI
class UIStackContainer:public Component
{
protected:
	std::unique_ptr<UIFont> mText;
	std::unique_ptr<UI_Primitive> mBackGround;
	
	DirectX::XMFLOAT3 mPosition = { 0,0,0.4f };//位置
	float mPadding[4] = {};

	DirectX::XMFLOAT2 mUIContainerSize = { 0.0f,0.0f };	//Container Size

	bool isContainerSizeChange = false;		//Container自体の大きさが変わったか
	bool isContainerPosChange = false;		//Containerの位置が変わったか

	bool isWordBoxChange = false;
	bool isWordAdaptive = false;

	UIPrimitiveConfig::UI_PrimitiveKind mKind;
public:

	UIStackContainer();
	~UIStackContainer() = default;

	void InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind kind);

	void LoadBackgroundTex(const char* filePath, DirectX::XMFLOAT2 size);
	void LoadBackgroundTex(std::shared_ptr<Texture> backgroundTex, DirectX::XMFLOAT2 size);

	void LoadFontTexture(const char* filePath, DirectX::XMFLOAT2 fontSize);
	void LoadFontTexture(std::shared_ptr<Texture> fontTex, DirectX::XMFLOAT2 fontSize);

	void SetContainerSize(float width, float height);
	void SetContainerSize(DirectX::XMFLOAT2 size);
	void SetContainerSize(float size);

	void SetText(const char* text);

	void SetPosZ(float posZ);
	void SetPosition(float x, float y);
	void InitPosition(float x, float y);

	void Update();

	void Draw();


	/// @brief Set mPadding param
	/// @param padding mPadding position
	/// @param param 
	void SetPadding(UIContainerConfig::Padding padding, float param);

	/// @brief Set All Padding
	/// @param param 
	void SetPadding(float param);

	void SetPadding(const float* param);

	/// @brief テクスチャなしの時の色設定
	/// @param color diffuse color
	void SetBackgroundColor(DirectX::XMFLOAT4 color);

	/// @brief Fontの色設定
	/// @param color diffuse color
	void SetFontColor(DirectX::XMFLOAT4 color);


protected:

	void NotifyWordBoxChangeListener();
	void ClearWordBoxChangeListener();
	bool GetWordBoxChange() { return isWordBoxChange; };

	void NotifySizeChangeListener();
	void ClearSizeChangeListener();

	void NotifyPositionChangeListener();
	void ClearPositionChangeListener();


	void UpdateContainerSize();
	void UpdateContainerPosition();

	void UpdateWordBox();
	void UpdateWordBoxSize();
	void UpdateWordPosition();

	void TriggerListener();
};


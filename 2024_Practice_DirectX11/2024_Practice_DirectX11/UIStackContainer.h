#pragma once
#include "SceneBase.h"
#include "UI_Font.h"

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
private:

#ifdef _DEBUG
	char mInputText[UITextOption::defaultMaxChar] = "";
	char mTemplateName[32] = "";

#endif

	struct UISet
	{
		std::unique_ptr<UI_Font> text;
		std::unique_ptr<UI_Primitive> background;
		std::unique_ptr<UI_Primitive> outline;
	};

	bool isContainerSizeChange = false;		//Container自体の大きさが変わったか
	bool isContainerPosChange = false;		//Containerの位置が変わったか

	bool isWordBoxChange = false;
	bool isWordAdaptive = false;

	std::string mObjName;


public:

	UISet mUiSet;

protected:

	bool isEditable = true;
	DirectX::XMFLOAT3 mPosition = { 0,0,0.9f };//位置
	DirectX::XMFLOAT2 mUIContainerSize = { 0.0f,0.0f };	//Container Size

	float mOutline = 1.0f;

	float mPadding[4] = {};
	float mMinBoxWidth = 0.f;
	float mMaxBoxWidth = 10000.f;
	bool isUseText = false;
	UIPrimitiveConfig::UI_PrimitiveKind mKind;

public:

	UIStackContainer();
	~UIStackContainer() override = default;

	void InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind kind);

	void LoadBackgroundTex(const char* filePath, DirectX::XMFLOAT2 size);
	void LoadBackgroundTex(const std::shared_ptr<Texture>& bgTex, DirectX::XMFLOAT2 texSize);

	void LoadFontTexture(const char* filePath, DirectX::XMFLOAT2 fontSize);
	void LoadFontTexture(const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 fontSize);

	void LoadFile(const std::shared_ptr<Texture>& bgTex,  const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 texSize = { 1,1 }, DirectX::XMFLOAT2 fontSize = { 1,1 });

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

	void SetOutlineColor(DirectX::XMFLOAT4 color);

	/// @brief Fontの色設定
	/// @param color diffuse color
	void SetFontColor(DirectX::XMFLOAT4 color);

	void DebugFunction();
	void SetEditable(bool isEditable) { this->isEditable = isEditable; };

	json SaveData(const char* objName);
	json SaveData();
	void LoadSaveData(json data, const char* objName);
	const DirectX::XMFLOAT2& GetContainerSize() const { return mUIContainerSize; };
	const DirectX::XMFLOAT3& GetPosition() const { return mPosition; };


	void SetObjName(const char* name) { mObjName = name; };
	const std::string& GetObjName() { return mObjName; };

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


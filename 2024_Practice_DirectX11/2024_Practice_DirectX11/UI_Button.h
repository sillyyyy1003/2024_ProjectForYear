#pragma once
#include "UIStackContainer.h"
#include "UI_Primitive.h"
#include "UI_Square.h"


class UI_Button :public UIComponent
{
private:
	//todo:this only was texture with rect need to evolution
	// îwåi
	std::unique_ptr<UIStackContainer> mContainer = nullptr;
	int mState = 0;

public:

	UI_Button() = default;
	~UI_Button() override;

	void LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);

	void Init(UIPrimitiveConfig::UI_PrimitiveKind primitiveKind, const std::shared_ptr<Texture>& bgTex, DirectX::XMFLOAT2 containerSize, const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 charSize);

	void Init(UIPrimitiveConfig::UI_PrimitiveKind primitiveKind, const std::shared_ptr<Texture>& bgTex, const std::shared_ptr<Texture>& fontTex);


	void Update();
	void Draw(int texSlot = 0);

	/// @brief Press 
	/// @return 
	bool isPressed();

	/// @brief Trigger
	/// @return 
	bool isTrigger();

	void LoadSaveData(json data, const char* objName);
	json SaveData(const char* objName);


protected:
	/// @brief É}ÉEÉXÇÃì¸óÕèàóùÇçsÇ§
	void PreUpdate();

	/// @brief ì¸óÕÇ…âûÇ∂ÇÈèàóù
	void GameUpdate();

	void SetPosZ(float Z) override;
	void SetPosition(DirectX::XMFLOAT2 pos) { mContainer->SetPosition(pos.x,pos.y); };
	void SetScale(DirectX::XMFLOAT2 scale) { mContainer->SetContainerSize(scale); };

};


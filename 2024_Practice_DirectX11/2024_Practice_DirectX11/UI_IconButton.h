#pragma once
#include <memory>
#include "D2D_UIBase.h"
#include "UI_Button.h"



class UI_IconButton : public Component
{
protected:

	//std::unique_ptr<D2D_UIBase> mButtonBackground;	
	std::unique_ptr<UI_Square>	mButtonIcon;	//Icon
	std::unique_ptr<UI_Square>	mBg;			//�w�i
	std::unique_ptr<UI_Square>	mShadow;		//�e

	DirectX::XMFLOAT2 mDefaultPosition = {};	//�����ʒu
	DirectX::XMFLOAT2 mShadowOffset = {};		//�e�̂��炵

	DirectX::XMFLOAT2 mPosition = {};
	float mPositionOffset = 10.0f;	//y���̂��炵
	DirectX::XMFLOAT2 mScale = {};

	std::string mObjName;
	int mState;

	RECT mRect = {};	//�����蔻��

public:
	UI_IconButton(){};
	~UI_IconButton() override = default;

	/// @brief IconButton�̏�����
	/// @param primitiveKind �{�^���̌`
	///	@param filePath Icon Tex �t�@�C���p�X
	///	@param objName �I�u�W�F�N�g��
	void Init(D2DUIConfig::UIShape primitiveKind,const char* filePath,const char* objName);

	/// @brief IconButton�̏�����
	/// @param bgTex �w�i�e�N�X�`��
	/// @param iconTex Icon�e�N�X�`��
	/// @param objName �I�u�W�F�N�g��
	virtual void Init(const char* bgTex, const char* iconTex, const char* objName);

	/// @brief �{�^���̈ʒu��ݒ肷��
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


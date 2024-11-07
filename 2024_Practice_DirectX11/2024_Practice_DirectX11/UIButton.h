#pragma once
#include "CanvasUI.h"

class UIButton :public CanvasUI
{
private:

	//�{�^���̃T�C�Y
	RECT mRect = { 0,0,0,0 };

	int mState = 0;

	DirectX::XMFLOAT4 mEffect = { 1,1,1,1 };

public:

	UIButton() = default;
	~UIButton() override;

	void Init(const char* _fileName);

	void Update(float dt) override;

	void Draw() override;

	/// @brief Press 
	/// @return 
	bool isPressed();

	/// @brief Trigger
	/// @return 
	bool isTrigger();

protected:

	/// @brief 
	void WriteShader() override;

	void LoadShader() override;

	/// @brief �}�E�X�̓��͏������s��
	/// @param dt 
	void PreUpdate(float dt);

	/// @brief ���͂ɉ����鏈��
	/// @param dt 
	void GameUpdate(float dt);

	/// @brief Render����
	/// @param dt 
	void LateUpdate(float dt);

	/// @brief �{�^���㉺��p�̈ʒu���擾
	/// @return 
	RECT GetRect();

	void InitAnimation();
};


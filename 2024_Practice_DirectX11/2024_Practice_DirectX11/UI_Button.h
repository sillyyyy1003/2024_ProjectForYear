#pragma once
#include "CanvasUI.h"

class UI_Button :public CanvasUI
{
private:

	//�{�^���̃T�C�Y
	RECT mRect = { 0,0,0,0 };

	int mState = 0;

	DirectX::XMFLOAT4 mEffect = { 1,1,1,1 };

public:

	UI_Button() = default;
	~UI_Button() override;

	void InitResource(const char* _fileName, DirectX::XMFLOAT2 size, bool isDefaultSize) override;

	void Update(float dt) override;

	void Draw() override;

	/// @brief ���̃{�^���������ꂽ���ǂ����H
	/// @return 
	bool isPressed();

private:
	/// @brief 
	void WriteShader() override;

	void LoadShader() override;

	/// @brief �}�E�X�̓��͏������s��
	/// @param dt 
	void PreUpdate(float dt);

	/// @brief ���͂ɉ����鏈��
	/// @param dt 
	void GameUpdate(float dt);

	/// @brief �{�^���㉺��p�̈ʒu���擾
	/// @return 
	RECT GetRect();
};


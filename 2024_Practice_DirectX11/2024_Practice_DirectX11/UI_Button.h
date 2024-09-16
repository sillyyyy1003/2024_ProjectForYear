#pragma once
#include "CanvasUI.h"

class UI_Button :public CanvasUI
{
private:

	//ボタンのサイズ
	RECT mRect = { 0,0,0,0 };

	int mState = 0;

	DirectX::XMFLOAT4 mEffect = { 1,1,1,1 };

public:

	UI_Button() = default;
	~UI_Button() override;

	void InitResource(const char* _fileName, DirectX::XMFLOAT2 size, bool isDefaultSize) override;

	void Update(float dt) override;

	void Draw() override;

	/// @brief このボタンが押されたかどうか？
	/// @return 
	bool isPressed();

private:
	/// @brief 
	void WriteShader() override;

	void LoadShader() override;

	/// @brief マウスの入力処理を行う
	/// @param dt 
	void PreUpdate(float dt);

	/// @brief 入力に応じる処理
	/// @param dt 
	void GameUpdate(float dt);

	/// @brief ボタン上下作用の位置を取得
	/// @return 
	RECT GetRect();
};


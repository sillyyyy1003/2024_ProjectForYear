#pragma once
#include "CanvasUI.h"

class UIButton :public CanvasUI
{
private:

	//ボタンのサイズ
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

	/// @brief マウスの入力処理を行う
	/// @param dt 
	void PreUpdate(float dt);

	/// @brief 入力に応じる処理
	/// @param dt 
	void GameUpdate(float dt);

	/// @brief Render処理
	/// @param dt 
	void LateUpdate(float dt);

	/// @brief ボタン上下作用の位置を取得
	/// @return 
	RECT GetRect();

	void InitAnimation();
};


#pragma once
#include "Ingredient.h"
#include "PBRModel.h"
#include "SceneBase.h"
#include "Primitive.h"
#include "UI2D.h"
#include "UIButton.h"
#include "UIFont.h"
#include "Water.h"



/// @brief メインのラブシーン
///	Potion Making Scene
class SceneGame :public SceneBase
{
private:


public:
	void Init();
	void UnInit();
	void Update(float dt);
	void Draw();

	/// @brief ゲームロジック用
	void TriggerListener();

	/// @brief
	///	@param dt delta time
	void GameObjectUpdate(float dt);
	
	void ChangeScene();

};
